#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS(Abstract)
class THISISSPARTA_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

	virtual void ActivateItem(AActor* Activator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 PointValue;
};
