#include "CoinItem.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = TEXT("DefaultCoin");
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag(TEXT("Player")))
	{
		DestroyItem();
	}
}
