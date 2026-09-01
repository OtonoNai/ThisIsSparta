#include "SpartaGameState.h"

#include "CoinItem.h"
#include "SmallCoinItem.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	TimeFormat.MinimumFractionalDigits = 2;
	TimeFormat.MaximumFractionalDigits = 2;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDTimerHandle,
		this,
		&ASpartaGameState::UpdateHUDWidget,
		0.05f,
		true);
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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->GetCurrentLevelIndex();
		}
	}
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	MaxWaveCount = 0;

	if (FoundVolumes.Num() > 0)
	{
		if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
		{
			MaxWaveCount = SpawnVolume->GetWaveCount();
		}
	}

	CurrentWaveCount = 0;
	StartWave();
}

void ASpartaGameState::OnLevelTimeUp()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	OnGameOver();
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (CurrentWaveCount + 1 < MaxWaveCount)
	{
		EndWave();
		return;
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			++CurrentLevelIndex;
			SpartaGameInstance->SetIndexToNextLevel(CurrentLevelIndex);
		}
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
	GetWorldTimerManager().ClearTimer(HUDTimerHandle);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUDWidget() const
{
	ASpartaPlayerController* SpartaPlayerController =
		Cast<ASpartaPlayerController>(GetWorld()->GetFirstPlayerController());

	if (!SpartaPlayerController)
	{
		return;
	}

	UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();

	if (!HUDWidget)
	{
		return;
	}

	if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
	{
		const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
		TimeText->SetText(FText::Format(INVTEXT("Time : {0}"), FText::AsNumber(RemainingTime, &TimeFormat)));
	}
	if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
	{
		LevelIndexText->SetText(FText::Format(INVTEXT("Level {0}"), CurrentLevelIndex + 1));
	}
	if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetGameInstance()))
		{
			ScoreText->SetText(
				FText::Format(INVTEXT("Score : {0}"), SpartaGameInstance->GetTotalScore()));
		}
	}
}

TSoftObjectPtr<UWorld> ASpartaGameState::GetFirstLevel() const
{
	return Levels.IsValidIndex(0) ? Levels[0] : nullptr;
}

void ASpartaGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.IsEmpty())
	{
		return;
	}

	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);

	if (!SpawnVolume)
	{
		return;
	}

	const int32 ItemToSpawnCount = SpawnVolume->GetWaveSpawnItemCount(CurrentWaveCount);

	for (int32 i = 0; i < ItemToSpawnCount; ++i)
	{
		AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

		if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
		{
			++SpawnedCoinCount;
		}
	}
	
	if (SpawnedCoinCount == 0)
    	{
    		AActor* ForcedCoin = SpawnVolume->SpawnItem(ASmallCoinItem::StaticClass());
    
    		if (ForcedCoin)
    		{
    			++SpawnedCoinCount;
    		}
    	}

	const int32 WaveTime = SpawnVolume->GetWaveTime(CurrentWaveCount);

	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작!"), CurrentWaveCount + 1);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Wave %d 시작!"), CurrentWaveCount + 1));
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		WaveTime,
		false);
}

void ASpartaGameState::EndWave()
{
	++CurrentWaveCount;
	StartWave();
}