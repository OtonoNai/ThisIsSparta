#include "BaseItem.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseItem::OnItemOverlap(AActor* OverlappedActor)
{
	
}

void ABaseItem::OnItemEndOverlap(AActor* OverlappedActor)
{
	
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}
