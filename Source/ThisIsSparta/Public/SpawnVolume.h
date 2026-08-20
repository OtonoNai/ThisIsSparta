#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class THISISSPARTA_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpawnVolume();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> ItemDataTable;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRandomItem();
	
	FItemSpawnRow* GetRandomItem() const;
	void SpawnItem(TSubclassOf<AActor> ItemClass);
	FVector GetRandomPoint() const;
};
