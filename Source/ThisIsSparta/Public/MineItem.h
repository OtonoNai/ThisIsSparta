#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class THISISSPARTA_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MineItem")
	TObjectPtr<USphereComponent> ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ExplosionDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UParticleSystem> ExplosionParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TObjectPtr<USoundBase> ExplosionSound;

	FTimerHandle ExplosionTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;
	void Explode();
	
private:
	bool bHasTriggered;
};
