#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USpartaGameInstance::InitializeGameData()
{
	CurrentLevelIndex = 0;
	TotalScore = 0;
}

void USpartaGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

int32 USpartaGameInstance::GetTotalScore() const
{
	return TotalScore;
}

void USpartaGameInstance::AdvanceToNextLevel()
{
	++CurrentLevelIndex;
}

int32 USpartaGameInstance::GetCurrentLevelIndex() const
{
	return CurrentLevelIndex;
}
