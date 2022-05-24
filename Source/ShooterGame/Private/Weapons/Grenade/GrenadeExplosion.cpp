// victorcas

#include "GrenadeExplosion.h"

AGrenadeExplosion::AGrenadeExplosion()
{
	PrimaryActorTick.bCanEverTick = true;

	// CAS TODO: overlap bind
	// Create sphere collision and bind collision overlap

	// CAS TODO: overlap trigger
	// on collision overlap: trace channel to see if wall in bentween
	// do damage
}

void AGrenadeExplosion::Init(APawn* NewOwner)
{
	PawnOwner = NewOwner;
}

void AGrenadeExplosion::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenadeExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

