#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

UCLASS()
class THISISSPARTA_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USpartaGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetTotalScore() const;
	
	void InitializeGameData();
	void AddToScore(int32 Amount);
	void AdvanceToNextLevel();
	int32 GetCurrentLevelIndex() const;

private:
	int32 TotalScore;
	int32 CurrentLevelIndex;
};
