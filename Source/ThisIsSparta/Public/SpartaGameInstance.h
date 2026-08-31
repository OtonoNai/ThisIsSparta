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

	UFUNCTION(Category = "Level")
	void InitializeGameData();
	UFUNCTION(Category = "Level")
	void AddToScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetTotalScore();
	UFUNCTION(Category = "Level")
	void SetIndexToNextLevel(int32 InCurrentLevelIndex);
	UFUNCTION(Category = "Level")
	int32 GetCurrentLevelIndex();

private:
	int32 TotalScore;
	int32 CurrentLevelIndex;
};
