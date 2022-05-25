// victorcas

#include "Grenade/Grenade.h"

#include "GrenadeExplosion.h"
#include "GrenadeManagerComp.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeMeshComp");
	if(IsValid(GrenadeMesh)) RootComponent = GrenadeMesh;
}

void AGrenade::Init(FVector NewImpulseVector, APawn* NewOwner)
{
	ImpulseVector = NewImpulseVector;
	PawnOwner = NewOwner;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GrenadeMesh) && ImpulseVector != FVector::ZeroVector)
	{
		GrenadeMesh->SetCollisionObjectType(COLLISION_PROJECTILE);
		GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GrenadeMesh->SetSimulatePhysics(true);
		GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GrenadeMesh->AddImpulse(UGrenadeManagerComp::GetImpulseVector(PawnOwner));
	}

	if(GrenadeData.ExplosionDelay > 0.0f)
	{
		GetWorldTimerManager().SetTimer(DelayGrenadeExplosionHandle, this,
			&AGrenade::ExplodeGrenade,
			GrenadeData.ExplosionDelay, false);
	}
}

void AGrenade::SpawnGrenadeExplosion()
{
	if(!IsValid(GrenadeExplosionClassToSpawn)) return;

	AGrenadeExplosion* GrenadeExplosion = Cast<AGrenadeExplosion>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this, GrenadeExplosionClassToSpawn, GetActorTransform(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn, PawnOwner));

	if(!IsValid(GrenadeExplosion)) return;
	GrenadeExplosion->Init(PawnOwner, this);
	UGameplayStatics::FinishSpawningActor(GrenadeExplosion, GetActorTransform());
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Spawn grenade explosion");

}

void AGrenade::ExplodeGrenade()
{
	if(GetWorldTimerManager().IsTimerActive(DelayGrenadeExplosionHandle))
	{
		GetWorldTimerManager().ClearTimer(DelayGrenadeExplosionHandle);
	}
	ExplodeGrenade_BP();

	SpawnGrenadeExplosion();
	
	if(GrenadeData.DestroyGrenadeAfterExplosionDelay > 0.0f)
	{
		GetWorldTimerManager().SetTimer(DelayDestroyGrenadeHandle, this,
			&AGrenade::DestroyGrenade,
			GrenadeData.DestroyGrenadeAfterExplosionDelay, false);
	}
}

void AGrenade::DestroyGrenade()
{
	if(GetWorldTimerManager().IsTimerActive(DelayDestroyGrenadeHandle))
	{
		GetWorldTimerManager().ClearTimer(DelayDestroyGrenadeHandle);
	}
	Destroy();
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, "Grenade flying");
}

