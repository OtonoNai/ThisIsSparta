#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class THISISSPARTA_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASpartaGameState();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Score")
	void OnCoinCollected();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnLevelTimeUp();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void UpdateHUDWidget();
	UFUNCTION(BlueprintCallable, Category = "Level")
	TSoftObjectPtr<UWorld> GetFirstLevel() const;

	void StartWave();
	void EndWave();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	TArray<TSoftObjectPtr<UWorld>> Levels;
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDTimerHandle;
	FNumberFormattingOptions TimeFormat;
	int32 Score;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	int32 SpawnedCoinCount;
	int32 CollectedCoinCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	float LevelDuration;
	int32 CurrentLevelIndex;
	int32 MaxLevel;
	int32 MaxWaveCount = 0;
	int32 CurrentWaveCount = 0;
};
