#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USpartaGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

void USpartaGameInstance::SetIndexToNextLevel(int32 InCurrentLevelIndex)
{
	CurrentLevelIndex = InCurrentLevelIndex;
}

int32 USpartaGameInstance::GetCurrentLevelIndex()
{
	return CurrentLevelIndex;
}
