// Copyright victorcas

#include "GrenadeManagerComp.h"

UGrenadeManagerComp::UGrenadeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrenadeManagerComp::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UGrenadeManagerComp::TickComponent(float DeltaTime, ELevelTick TickType,
										FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrenadeManagerComp::CancelCurrentThrow()
{
	if(!IsValid(CurrentEquippedGrenade)) return;
	CurrentEquippedGrenade->OnHoldCancel();
}

void UGrenadeManagerComp::EquipWeapon()
{
	// CAS TODO:
	// call equip net weapon function
	// make equipped grenade null
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Equip weapon");
}

void UGrenadeManagerComp::EquipNextGrenade()
{
	// CAS TODO:
	// if !equipped grenade -> equip first grenade
	// else
	// cycle through grenade array
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Equip grenade");
}

void UGrenadeManagerComp::HoldGrenadeButton()
{
	if(IsGrenadeEquipped())
	{
		if(GetOwner()->GetWorldTimerManager().IsTimerActive(HoldToEquipWeaponHandle)) return;
		GetOwner()->GetWorldTimerManager().SetTimer(HoldToEquipWeaponHandle, this,
																		&UGrenadeManagerComp::ReleaseGrenadeButton,
																		TimeHoldToEquipWeapon, true);
	}
	else
	{
		EquipNextGrenade();
	}
}

void UGrenadeManagerComp::ReleaseGrenadeButton()
{
	if(!GetOwner()->GetWorldTimerManager().IsTimerActive(HoldToEquipWeaponHandle)) return;
	if(GetOwner()->GetWorldTimerManager().GetTimerElapsed(HoldToEquipWeaponHandle) >= TimeHoldToEquipWeapon)
	{
		EquipWeapon();
	}
	else
	{
		EquipNextGrenade();
	}
	GetOwner()->GetWorldTimerManager().ClearTimer(HoldToEquipWeaponHandle);
}
