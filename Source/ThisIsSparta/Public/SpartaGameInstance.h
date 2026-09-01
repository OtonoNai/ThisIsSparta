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
	int32 GetTotalScore() const;
	UFUNCTION(Category = "Level")
	void AdvanceToNextLevel();
	UFUNCTION(Category = "Level")
	int32 GetCurrentLevelIndex() const;

private:
	int32 TotalScore;
	int32 CurrentLevelIndex;
};
