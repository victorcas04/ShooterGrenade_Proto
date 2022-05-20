// Copyright victorcas

#include "GrenadeManagerComp.h"

// Sets default values for this component's properties
UGrenadeManagerComp::UGrenadeManagerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrenadeManagerComp::BeginPlay()
{
	Super::BeginPlay();
	
	SetComponentTickEnabled(false);
}

void UGrenadeManagerComp::EquipWeapon()
{
	// CAS TODO:
}

void UGrenadeManagerComp::SwapGrenade()
{
	// CAS TODO:
	// cycle through grenade array
}

// Called every frame
void UGrenadeManagerComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UGrenadeManagerComp::HoldGrenadeButton()
{
	// if !grenade equipped -> equip grenade
	// else
	// set timer with hold time to equip weapon -> ReleaseGrenadeButton 
}

void UGrenadeManagerComp::ReleaseGrenadeButton()
{
	// CAS TODO:
	// if timer handle is !valid -> return
	// timer handle elapsed time >= hold time to equip weapon -> equip weapon
	// else -> swap grenades
	// clear timer handle
}
