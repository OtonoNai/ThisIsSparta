#include "HealingItem.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}
