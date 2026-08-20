#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class THISISSPARTA_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual void OnItemOverlap(AActor* OverlappedActor) = 0;
	virtual void OnItemEndOverlap(AActor* OverlappedActor) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;
};
