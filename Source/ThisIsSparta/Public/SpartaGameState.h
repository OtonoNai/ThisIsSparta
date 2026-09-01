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

	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
	void OnGameOver();
	
	void StartWave();
	void EndWave();
	
	void ReportScoreGained(int32 Amount);
	void OnCoinCollected();
	
	void UpdateHUDWidget() const;

	TSoftObjectPtr<UWorld> GetFirstLevel() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UWorld>> Levels;
	UPROPERTY(BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = true))
	TObjectPtr<ASpawnVolume> SpawnVolume;
	UPROPERTY(BlueprintReadOnly, Category = "Level", meta=(AllowPrivateAccess = true))
	int32 SpawnedCoinCount;
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta=(AllowPrivateAccess = true))
	float HUDUpdateInterval = 0.05f;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDTimerHandle;
	FNumberFormattingOptions TimeFormat;
	int32 CollectedCoinCount;
	int32 CurrentLevelIndex;
	int32 WaveCount = 0;
	int32 CurrentWaveIndex = 0;
};
