#include "SpartaAICharacter.h"

#include "SpartaAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"

ASpartaAICharacter::ASpartaAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = ASpartaAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = WalkSpeed;
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
}

void ASpartaAICharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = NewSpeed;
	}
}

void ASpartaAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

