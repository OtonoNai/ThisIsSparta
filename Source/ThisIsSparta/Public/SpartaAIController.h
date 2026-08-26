#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SpartaAIController.generated.h"

class UAISenseConfig_Sight;
class UAIPerceptionComponent;

UCLASS()
class THISISSPARTA_API ASpartaAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASpartaAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(class APawn* InPawn) override;
	
	UFUNCTION()
	void MoveToRandomLocation();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MoveRadius = 1000.0f;
	
	FTimerHandle RandomMoveTimer;
};
