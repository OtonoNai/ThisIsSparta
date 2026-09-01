#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

class ASpawnVolume;

UCLASS()
class THISISSPARTA_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASpartaGameState();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void ReportScoreGained(int32 Amount);
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
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category = "Level")
	TSoftObjectPtr<UWorld> GetFirstLevel() const;

	void StartWave();
	void EndWave();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	TArray<TSoftObjectPtr<UWorld>> Levels;
	UPROPERTY(BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<ASpawnVolume> SpawnVolume;
	UPROPERTY(BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = "true"))
	int32 SpawnedCoinCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta=(AllowPrivateAccess="true"))
	float HUDUpdateInterval = 0.05f;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDTimerHandle;
	FNumberFormattingOptions TimeFormat;
	int32 CollectedCoinCount;
	int32 CurrentLevelIndex;
	int32 WaveCount = 0;
	int32 CurrentWaveIndex = 0;
};
