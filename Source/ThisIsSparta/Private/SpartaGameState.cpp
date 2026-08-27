#include "SpartaGameState.h"

#include "CoinItem.h"
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
	MaxLevel = 3;
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
		0.01f,
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

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 20;

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
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	OnGameOver();
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

void ASpartaGameState::UpdateHUDWidget()
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
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
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
	return Levels[0];
}
