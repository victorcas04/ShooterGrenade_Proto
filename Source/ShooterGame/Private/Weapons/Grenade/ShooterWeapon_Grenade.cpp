// Copyright victorcas

#include "Grenade/ShooterWeapon_Grenade.h"
#include "GrenadeManagerComp.h"
#include "GrenadeTestHelpers.h"
#include "Grenade/Grenade.h"

AShooterWeapon_Grenade::AShooterWeapon_Grenade(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

EAmmoType AShooterWeapon_Grenade::GetAmmoType() const
{
	{
		const EGrenadeType GType = GrenadeClassToSpawn.GetDefaultObject()->GrenadeData.GrenadeType;
		switch (GType)
		{
		case EGrenadeType::EBouncing: return EAmmoType::EBouncingGrenade;
		case EGrenadeType::ESticky: return EAmmoType::EStickyGrenade;
		default: ;
		}
		return EAmmoType::EMax;
	}
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
	// CAS TODO: start reload
}

void AShooterWeapon_Grenade::StopReload()
{
	// CAS TODO: stop reload
}

void AShooterWeapon_Grenade::ReloadWeapon()
{
	// CAS TODO: reload action
}

void AShooterWeapon_Grenade::FireWeapon()
{
	if(IsValid(GrenadeClassToSpawn))
	{		
		const FTransform SpawnTransform = UGrenadeManagerComp::GetGrenadeSpawnTransformPoint(GetPawnOwner());
		AGrenade* Grenade = Cast<AGrenade>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this, GrenadeClassToSpawn, SpawnTransform,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn, GetPawnOwner()));

		if(!IsValid(Grenade)) return;
		Grenade->Init(UGrenadeManagerComp::GetImpulseVector(GetPawnOwner()), GetPawnOwner());
		UGameplayStatics::FinishSpawningActor(Grenade, SpawnTransform);
		
		// CAS TODO: remove uses
		
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Spawn grenade with trajectory parameter");
	}
	
}

void AShooterWeapon_Grenade::OnBurstStarted()
{
	// CAS TODO: burst start
}

void AShooterWeapon_Grenade::OnBurstFinished()
{
	// CAS TODO: burst finish
	Super::OnBurstFinished();
}

void AShooterWeapon_Grenade::OnHoldStart()
{
	// TODO: Hold animation (must be looping)
	
	if(!IsValid(GetPawnOwner()) ||
		GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Show trajectory");
	GetPawnOwner()->GetWorldTimerManager().SetTimer(RecalculateTrajectoryHandle, this,
																	&AShooterWeapon_Grenade::OnHoldLoop,
																	TrajectoryData.DelayRecalculateTrajectory, true);
	
	bCanShoot = true;
}

void AShooterWeapon_Grenade::OnHoldLoop()
{
	GTestEObjectIsValid ValidComp;
	auto ActorComp = GrenadeTestHelpers::GetValidatedComponentByClass(
		GetPawnOwner(), UGrenadeManagerComp::StaticClass(), ValidComp);
		
	if(ValidComp == GTestEObjectIsValid::Valid)
	{
		Cast<UGrenadeManagerComp>(ActorComp)->ShowTrajectory(true);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, TrajectoryData.DelayRecalculateTrajectory, FColor::Green, "Recalculate trajectory");
}

void AShooterWeapon_Grenade::OnHoldRelease()
{
		
	// TODO: Throw grenade anim
	
	if(bCanShoot) FireWeapon();
	OnHoldCancel();
}

void AShooterWeapon_Grenade::OnHoldCancel()
{
	
	// TODO: Back to idle anim
	
	bCanShoot = false;
	if(!IsValid(GetPawnOwner()) ||
		!GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;
	
	GetPawnOwner()->GetWorldTimerManager().ClearTimer(RecalculateTrajectoryHandle);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Hide trajectory");

	GTestEObjectIsValid ValidComp;
	auto ActorComp = GrenadeTestHelpers::GetValidatedComponentByClass(
		GetPawnOwner(), UGrenadeManagerComp::StaticClass(), ValidComp);
		
	if(ValidComp == GTestEObjectIsValid::Valid)
	{
		Cast<UGrenadeManagerComp>(ActorComp)->ShowTrajectory(false);
	}
}
