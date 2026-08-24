#include "HealingItem.h"

#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
	{
		PlayerCharacter->AddHealth(HealAmount);
		
		DestroyItem();
	}
}
