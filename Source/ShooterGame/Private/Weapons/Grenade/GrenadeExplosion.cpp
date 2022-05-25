// victorcas

#include "GrenadeExplosion.h"

#include "GrenadeTestHelpers.h"

AGrenadeExplosion::AGrenadeExplosion()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentDamage = GrenadeExplosionData.ExplosionDamage;
	TriggerExplosion = CreateDefaultSubobject<USphereComponent>("TriggerExplosionComp");
	if(IsValid(TriggerExplosion))
	{
		TriggerExplosion->SetCollisionProfileName("Trigger");
	}
}

void AGrenadeExplosion::Init(APawn* NewOwner, AActor* GrenadeSpawner)
{
	PawnOwner = NewOwner;
	if(IsValid(GrenadeSpawner)) DamageActorsToIgnore.AddUnique(GrenadeSpawner);
	if(!GrenadeExplosionData.bCanDamageOwner)
	{
		if(IsValid(PawnOwner)) DamageActorsToIgnore.AddUnique(PawnOwner);
	}
}

void AGrenadeExplosion::BeginPlay()
{
	Super::BeginPlay();

	
	if(IsValid(TriggerExplosion))
	{
		TriggerExplosion->SetHiddenInGame(!GrenadeExplosionData.bShowTriggerSphere);
		TriggerExplosion->OnComponentBeginOverlap.AddDynamic(this, &AGrenadeExplosion::OnTriggerExplosionEnter);
		TriggerExplosion->OnComponentEndOverlap.AddDynamic(this, &AGrenadeExplosion::OnTriggerExplosionExit);
		TriggerExplosion->SetSphereRadius(GrenadeExplosionData.ExplosionRadius);
	}

	ExplosionVFX_BP();
	ExplosionSFX_BP();
	
	if(GrenadeExplosionData.bExplosionPersists)
	{
		CurrentDamage = GrenadeExplosionData.PersistDamage;
		if(GrenadeExplosionData.DelayBeforeExplosionEnds > .0f)
		{
			GetWorldTimerManager().SetTimer(DelayBeforeExplosionEndsHandle,
				this, &AGrenadeExplosion::DestroyGrenadeExplosion,
				GrenadeExplosionData.DelayBeforeExplosionEnds, false);
		}
	}
	else
	{
		DestroyGrenadeExplosion();
	}
}

void AGrenadeExplosion::OnTriggerExplosionEnter(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(CurrentDamage > .0f)
	{
		if(!GrenadeTestHelpers::ExistsWallBetweenActors(this, OtherActor))
		{
			UGameplayStatics::ApplyRadialDamage(this,
				CurrentDamage, GetActorLocation(),
				GrenadeExplosionData.ExplosionRadius, GrenadeExplosionData.DamageType,
				DamageActorsToIgnore,
				this, PawnOwner->GetController());
			ExplosionDamagedActor_BP(OtherActor, CurrentDamage);
		}
	}
}

void AGrenadeExplosion::OnTriggerExplosionExit(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AGrenadeExplosion::DestroyGrenadeExplosion()
{
	if(IsValid(TriggerExplosion))
	{
		if(TriggerExplosion->OnComponentBeginOverlap.IsBound())
		{
			TriggerExplosion->OnComponentBeginOverlap.RemoveDynamic(this, &AGrenadeExplosion::OnTriggerExplosionEnter);
		}
		if(TriggerExplosion->OnComponentEndOverlap.IsBound())
		{
			TriggerExplosion->OnComponentEndOverlap.RemoveDynamic(this, &AGrenadeExplosion::OnTriggerExplosionExit);
		}
	}
	if(GetWorldTimerManager().IsTimerActive(DelayBeforeExplosionEndsHandle))
	{
		GetWorldTimerManager().ClearTimer(DelayBeforeExplosionEndsHandle);
	}
	Destroy();
}

void AGrenadeExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

