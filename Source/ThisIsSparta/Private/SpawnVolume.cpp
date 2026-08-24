#include "SpawnVolume.h"

#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(Scene);
	
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->SetupAttachment(Scene);
	
	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPoint() const
{
	FVector BoxExtent = SpawnVolume->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnVolume->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass)
	{
		return nullptr;
	}
	
	return GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPoint(),
		FRotator::ZeroRotator
		);
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (TObjectPtr<UClass> ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable)
	{
		return nullptr;
	}
	
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);
	
	if (AllRows.IsEmpty())
	{
		return nullptr;
	}
	
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float Accumulate = 0.0f;
	for (FItemSpawnRow* Row : AllRows)
	{
		Accumulate += Row->SpawnChance;
		if (RandValue <= Accumulate)
		{
			return Row;
		}
	}
	
	return nullptr;
}
