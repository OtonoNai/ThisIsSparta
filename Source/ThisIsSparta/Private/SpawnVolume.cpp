#include "SpawnVolume.h"

#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(Scene);
	
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->SetupAttachment(Scene);
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

void ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass)
	{
		return;
	}
	
	GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPoint(),
		FRotator::ZeroRotator
		);
}
