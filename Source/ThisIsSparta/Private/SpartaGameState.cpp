#include "SpartaGameState.h"

#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevel = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	Score += Amount;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::OnCoinCollected()
{
	++CollectedCoinCount;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->GetCurrentLevelIndex();
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					++SpawnedCoinCount;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false);
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			++CurrentLevelIndex;
			SpartaGameInstance->SetIndexToNextLevel(CurrentLevelIndex);
			AddScore(Score);
		}
	}

	if (CurrentLevelIndex > MaxLevel)
	{
		OnGameOver();
		return;
	}

	if (Levels.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, Levels[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
}
