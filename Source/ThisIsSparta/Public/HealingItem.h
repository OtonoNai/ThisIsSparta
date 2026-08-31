#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class THISISSPARTA_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();

	virtual void ActivateItem(AActor* Activator) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 HealAmount;
};
