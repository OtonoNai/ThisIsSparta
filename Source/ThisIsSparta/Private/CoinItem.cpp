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
	Super::ActivateItem(Activator);

	if (!Cast<ASpartaCharacter>(Activator))
	{
		return;
	}

	if (ASpartaGameState* GameState = GetWorld()->GetGameState<ASpartaGameState>())
	{
		GameState->ReportScoreGained(PointValue);
		GameState->OnCoinCollected();
	}

	DestroyItem();
}
