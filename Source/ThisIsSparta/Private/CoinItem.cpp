#include "CoinItem.h"

#include "SpartaCharacter.h"
#include "SpartaGameStateBase.h"
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
			if (ASpartaGameStateBase* GameState = World->GetGameState<ASpartaGameStateBase>())
			{
				GameState->AddScore(PointValue);
			}
		}
		DestroyItem();
	}
}
