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
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
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
		HUDUpdateInterval,
		true);
}

void ASpartaGameState::ReportScoreGained(int32 Amount)
{
	if (USpartaGameInstance* SpartaGameInstance = GetGameInstance<USpartaGameInstance>())
	{
		SpartaGameInstance->AddToScore(Amount);
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
	if (ASpartaPlayerController* SpartaPlayerController =
		Cast<ASpartaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		SpartaPlayerController->ShowGameHUD();
	}

	if (USpartaGameInstance* SpartaGameInstance = GetGameInstance<USpartaGameInstance>())
	{
		CurrentLevelIndex = SpartaGameInstance->GetCurrentLevelIndex();
	}

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	SpawnVolume = nullptr;

	WaveCount = 0;

	if (FoundVolumes.Num() > 0)
	{
		SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{
			WaveCount = SpawnVolume->GetWaveCount();
		}
	}

	CurrentWaveIndex = 0;
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

	if (CurrentWaveIndex + 1 < WaveCount)
	{
		EndWave();
		return;
	}

	if (USpartaGameInstance* SpartaGameInstance = GetGameInstance<USpartaGameInstance>())
	{
		++CurrentLevelIndex;
		SpartaGameInstance->AdvanceToNextLevel();
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

	if (ASpartaPlayerController* SpartaPlayerController
		= GetWorld()->GetFirstPlayerController<ASpartaPlayerController>())
	{
		SpartaPlayerController->SetPause(true);
		SpartaPlayerController->ShowMainMenu(true);
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
		if (USpartaGameInstance* SpartaGameInstance = GetGameInstance<USpartaGameInstance>())
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

	if (!IsValid(SpawnVolume))
	{
		return;
	}

	const int32 ItemToSpawnCount = SpawnVolume->GetWaveSpawnItemCount(CurrentWaveIndex);

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

	const int32 WaveTime = SpawnVolume->GetWaveTime(CurrentWaveIndex);

	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작!"), CurrentWaveIndex + 1);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Wave %d 시작!"), CurrentWaveIndex + 1));
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
	++CurrentWaveIndex;

	FTimerHandle NextWaveHandle;
	GetWorldTimerManager().SetTimer(
		NextWaveHandle,
		this,
		&ASpartaGameState::StartWave,
		0.01f,
		false);
}
