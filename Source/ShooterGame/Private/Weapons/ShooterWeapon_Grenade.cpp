// Copyright victorcas

#include "Weapons/ShooterWeapon_Grenade.h"

AShooterWeapon_Grenade::AShooterWeapon_Grenade(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void AShooterWeapon_Grenade::Destroyed()
{
	OnHoldCancel();
}

void AShooterWeapon_Grenade::StartFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Pressed Fire");
	OnHoldStart();
}

void AShooterWeapon_Grenade::StopFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Released Fire");
	OnHoldRelease();
}

void AShooterWeapon_Grenade::StartReload(bool bFromReplication)
{
	
}

void AShooterWeapon_Grenade::StopReload()
{
	
}

void AShooterWeapon_Grenade::ReloadWeapon()
{
	
}

void AShooterWeapon_Grenade::FireWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Spawn grenade with trajectory parameter");
}

void AShooterWeapon_Grenade::OnBurstStarted()
{
	
}

void AShooterWeapon_Grenade::OnBurstFinished()
{
	Super::OnBurstFinished();
}

void AShooterWeapon_Grenade::OnHoldStart()
{
	if(!IsValid(GetPawnOwner()) ||
		GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Show trajectory");
	GetPawnOwner()->GetWorldTimerManager().SetTimer(RecalculateTrajectoryHandle, this,
																	&AShooterWeapon_Grenade::OnHoldLoop,
																	DelayRecalculateTrajectory, true);
}

void AShooterWeapon_Grenade::OnHoldLoop()
{
	GEngine->AddOnScreenDebugMessage(-1, DelayRecalculateTrajectory, FColor::Green, "Recalculate trajectory");
	// CAS TODO:
	// trajectory var = new trajectory
}

void AShooterWeapon_Grenade::OnHoldRelease()
{
	FireWeapon();
	OnHoldCancel();
}

void AShooterWeapon_Grenade::OnHoldCancel()
{
	if(!IsValid(GetPawnOwner()) ||
		!GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Hide trajectory");
	GetPawnOwner()->GetWorldTimerManager().ClearTimer(RecalculateTrajectoryHandle);
}
