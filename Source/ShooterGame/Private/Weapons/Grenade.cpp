// victorcas

#include "Grenade.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, "Grenade following trajectory");
}

