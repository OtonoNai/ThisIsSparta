#include "HealingItem.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag(TEXT("Player")))
	{
		DestroyItem();
	}
}
