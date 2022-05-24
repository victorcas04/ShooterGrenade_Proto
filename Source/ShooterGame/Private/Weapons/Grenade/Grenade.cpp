// victorcas

#include "Grenade/Grenade.h"

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
		//GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GrenadeMesh->AddImpulse(UGrenadeManagerComp::GetImpulseVector(PawnOwner));
	}
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, "Grenade flying");
}

