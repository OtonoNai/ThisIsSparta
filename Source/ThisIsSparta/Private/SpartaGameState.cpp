#include "SpartaGameState.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	Score += Amount;
}
