#include "CoinItem.h"

#include "SpartaCharacter.h"
#include "SpartaGameState.h"
#include "Engine/World.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = TEXT("DefaultCoin");
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Cast<ASpartaCharacter>(Activator))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}
