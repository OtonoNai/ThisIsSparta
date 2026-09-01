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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = true))
	int32 HealAmount;
};
