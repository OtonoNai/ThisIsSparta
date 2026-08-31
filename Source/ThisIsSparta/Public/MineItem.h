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

	virtual void ActivateItem(AActor* Activator) override;
	void Explode();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MineItem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 ExplosionDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ExplosionParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ExplosionSound;

	FTimerHandle ExplosionTimerHandle;
	bool bHasTriggered;
};
