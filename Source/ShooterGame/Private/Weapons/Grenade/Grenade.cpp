// victorcas

#include "Grenade/Grenade.h"

#include "GrenadeManagerComp.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
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
		GrenadeMesh->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel2);
		GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GrenadeMesh->SetSimulatePhysics(true);
		GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GrenadeMesh->AddImpulse(UGrenadeManagerComp::GetImpulseVector(PawnOwner));
	}
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, "Grenade flying");
}

