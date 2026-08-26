#include "SpartaAIController.h"

#include "NavigationSystem.h"

ASpartaAIController::ASpartaAIController()
{
}

void ASpartaAIController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		RandomMoveTimer,
		this,
		&ASpartaAIController::MoveToRandomLocation,
		3.0f,
		true,
		1.0f);
}

void ASpartaAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!InPawn)
	{
		return;
	}
}

void ASpartaAIController::MoveToRandomLocation()
{
	APawn* MyPawn = GetPawn();
	
	if (!MyPawn)
	{
		return;
	}
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSys)
	{
		return;
	}
	
	FNavLocation RandomLocation;
	bool bFoundLocation = NavSys->GetRandomPointInNavigableRadius(
		MyPawn->GetActorLocation(),
		MoveRadius,
		RandomLocation);
	
	if (bFoundLocation)
	{
		MoveToLocation(RandomLocation.Location);
	}
}


