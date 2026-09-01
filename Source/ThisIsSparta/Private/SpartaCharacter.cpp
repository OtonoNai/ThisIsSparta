#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "SpartaGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	OverheadHealth = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadHealth"));
	OverheadHealth->SetupAttachment(RootComponent);
	OverheadHealth->SetWidgetSpace(EWidgetSpace::Screen);

	HealthUIFormat.MinimumFractionalDigits = 0;
	HealthUIFormat.MaximumFractionalDigits = 0;
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHealth();
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHealth();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	PlayerController->BindInputActions(EnhancedInput, this);
}

float ASpartaCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	UpdateOverheadHealth();

	return ActualDamage;
}

void ASpartaCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D MoveInput = InputActionValue.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::EndJump(const FInputActionValue& InputActionValue)
{
	if (!InputActionValue.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& InputActionValue)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SprintSpeedMultiplier;
	}
}

void ASpartaCharacter::EndSprint(const FInputActionValue& InputActionValue)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHealth() const
{
	if (!OverheadHealth)
	{
		return;
	}

	UUserWidget* OverheadHPInstance = OverheadHealth->GetUserWidgetObject();
	if (!OverheadHPInstance)
	{
		return;
	}

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadHPInstance->GetWidgetFromName(TEXT("HPValue"))))
	{
		HPText->SetText(
			FText::Format(
				INVTEXT("{0} / {1}"),
				FText::AsNumber(Health, &HealthUIFormat),
				FText::AsNumber(MaxHealth, &HealthUIFormat)));
	}
}
