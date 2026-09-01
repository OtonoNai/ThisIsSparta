#include "SpartaPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaCharacter.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		if (ASpartaGameState* SpartaGameState = GetWorld()->GetGameState<ASpartaGameState>())
		{
			SpartaGameState->UpdateHUDWidget();
		}
	}
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (!MainMenuWidgetClass)
	{
		return;
	}

	MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	if (!MainMenuWidgetInstance)
	{
		return;
	}

	MainMenuWidgetInstance->AddToViewport();

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("Start")));
	if (!ButtonText)
	{
		return;
	}

	ButtonText->SetText(FText::FromString(bIsRestart ? TEXT("Restart") : TEXT("Start")));

	if (!bIsRestart)
	{
		return;
	}

	UFunction* PlayAnimFunction = MainMenuWidgetInstance->FindFunction(FName(TEXT("PlayGameOverAnim")));
	if (PlayAnimFunction)
	{
		MainMenuWidgetInstance->ProcessEvent(PlayAnimFunction, nullptr);
	}

	UTextBlock* TotalScoreText =
		Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TotalScore")));
	if (!TotalScoreText)
	{
		return;
	}

	USpartaGameInstance* SpartaGameInstance = GetWorld()->GetGameInstance<USpartaGameInstance>();
	if (!SpartaGameInstance)
	{
		return;
	}

	TotalScoreText->SetText(
		FText::Format(INVTEXT("Score : {0}"), SpartaGameInstance->GetTotalScore()));
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = GetWorld()->GetGameInstance<USpartaGameInstance>())
	{
		SpartaGameInstance->InitializeGameData();
	}

	if (ASpartaGameState* SpartaGameState = GetWorld()->GetGameState<ASpartaGameState>())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, SpartaGameState->GetFirstLevel());
	}

	SetPause(false);
}

void ASpartaPlayerController::BindInputActions(UEnhancedInputComponent* EnhancedInput, ASpartaCharacter* InCharacter)
{
	if (MoveAction)
	{
		EnhancedInput->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			InCharacter,
			&ASpartaCharacter::Move);
	}
	if (JumpAction)
	{
		EnhancedInput->BindAction(
			JumpAction,
			ETriggerEvent::Triggered,
			InCharacter,
			&ASpartaCharacter::StartJump);
		EnhancedInput->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			InCharacter,
			&ASpartaCharacter::EndJump);
	}
	if (LookAction)
	{
		EnhancedInput->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			InCharacter,
			&ASpartaCharacter::Look);
	}
	if (SprintAction)
	{
		EnhancedInput->BindAction(
			SprintAction,
			ETriggerEvent::Triggered,
			InCharacter,
			&ASpartaCharacter::StartSprint);
		EnhancedInput->BindAction(
			SprintAction,
			ETriggerEvent::Completed,
			InCharacter,
			&ASpartaCharacter::EndSprint);
	}
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	const FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains(TEXT("MenuLevel")))
	{
		ShowMainMenu(false);
	}
}
