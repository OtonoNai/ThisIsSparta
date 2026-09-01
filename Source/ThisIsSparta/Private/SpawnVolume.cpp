#include "SpawnVolume.h"

#include "LevelWaveRow.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPoint() const
{
	const FVector BoxExtent = SpawnBox->GetScaledBoxExtent();
	const FVector BoxOrigin = SpawnBox->GetComponentLocation();

	return BoxOrigin + FVector(
		       FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		       FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		       FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}

int32 ASpawnVolume::GetWaveCount() const
{
	if (!WaveDataTable)
	{
		return 0;
	}

	TArray<FLevelWaveRow*> AllRows;
	static const FString ContextString(TEXT("LevelWaveContext"));
	WaveDataTable->GetAllRows(ContextString, AllRows);

	return AllRows.Num();
}

int32 ASpawnVolume::GetWaveTime(int32 Index) const
{
	const FLevelWaveRow* Row = GetWaveRow(Index);

	return Row ? Row->WaveTime : 0;
}

int32 ASpawnVolume::GetWaveSpawnItemCount(int32 Index) const
{
	const FLevelWaveRow* Row = GetWaveRow(Index);

	return Row ? Row->SpawnItemCount : 0;
}

const FLevelWaveRow* ASpawnVolume::GetWaveRow(int32 Index) const
{
	if (!WaveDataTable)
	{
		return nullptr;
	}

	TArray<FLevelWaveRow*> AllRows;
	static const FString ContextString(TEXT("LevelWaveContext"));
	WaveDataTable->GetAllRows(ContextString, AllRows);

	if (!AllRows.IsValidIndex(Index))
	{
		return nullptr;
	}

	return AllRows[Index];
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
		FRotator::ZeroRotator);
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItemRow())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItemRow() const
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
		if (RandValue < Accumulate)
		{
			return Row;
		}
	}

	return nullptr;
}
