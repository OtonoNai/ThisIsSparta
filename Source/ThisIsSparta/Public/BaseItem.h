// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class THISISSPARTA_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	virtual void OnItemOverlap(AActor* OverlappedActor) override;
	virtual void OnItemEndOverlap(AActor* OverlappedActor) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem();
	virtual FName GetItemType() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName ItemType;
};
