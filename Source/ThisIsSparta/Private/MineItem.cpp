#include "MineItem.h"

#include "SpartaCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ItemType = "Mine";
	ExplosionDelay = 3.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 40;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(SceneComponent);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasTriggered)
	{
		return;
	}

	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle,
	                                       this,
	                                       &AMineItem::Explode,
	                                       ExplosionDelay,
	                                       false);

	bHasTriggered = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Cast<ASpartaCharacter>(Actor))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass());
		}
	}

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation());
	}

	if (Particle)
	{
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		FTimerHandle ParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			ParticleTimerHandle,
			[WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false);
	}

	Destroy();
}
