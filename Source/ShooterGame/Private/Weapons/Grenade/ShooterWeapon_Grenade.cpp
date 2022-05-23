// Copyright victorcas

#include "Grenade/ShooterWeapon_Grenade.h"
#include "Grenade/Grenade.h"

AShooterWeapon_Grenade::AShooterWeapon_Grenade(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

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
	// CAS TODO:
}

void AShooterWeapon_Grenade::StopReload()
{
	// CAS TODO:
}

void AShooterWeapon_Grenade::ReloadWeapon()
{
	// CAS TODO:
}

void AShooterWeapon_Grenade::FireWeapon()
{
	// CAS TODO:
	// add trajectory parameter and make grenade follow it
	if(IsValid(GrenadeClassToSpawn))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AGrenade* Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClassToSpawn, SpawnInfo);
		
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Spawn grenade with trajectory parameter");
	}
	
}

void AShooterWeapon_Grenade::OnBurstStarted()
{
	// CAS TODO:
}

void AShooterWeapon_Grenade::OnBurstFinished()
{
	// CAS TODO:
	Super::OnBurstFinished();
}

void AShooterWeapon_Grenade::OnHoldStart()
{
	// CAS TODO:
	// Show trajectory
	// Hold animation (must be looping)
	
	if(!IsValid(GetPawnOwner()) ||
		GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Show trajectory");
	GetPawnOwner()->GetWorldTimerManager().SetTimer(RecalculateTrajectoryHandle, this,
																	&AShooterWeapon_Grenade::OnHoldLoop,
																	TrajectoryConfig.DelayRecalculateTrajectory, true);
	
	bCanShoot = true;
}

void AShooterWeapon_Grenade::OnHoldLoop()
{
	
	// CAS TODO:
	// Recalculate trajectory

	/*
	FVector Velocity = FVector(1.0f, 1.0f, 1.0f) * TrajectoryConfig.ThrowForce;
	FPredictProjectilePathParams Params = FPredictProjectilePathParams(TrajectoryConfig.ProjectileRadius, GetPawnOwner()->GetActorLocation(), Velocity,
		TrajectoryConfig.MaxSimTime, TrajectoryConfig.ObjectTypes, TrajectoryConfig.ActorsToIgnore);
	FPredictProjectilePathParams
	*/

	/*
	FPredictProjectilePathParams Params = FPredictProjectilePathParams(ProjectileRadius, StartPos, LaunchVelocity, MaxSimTime);
	Params.bTraceWithCollision = bTracePath;
	Params.bTraceComplex = bTraceComplex;
	Params.ActorsToIgnore = ActorsToIgnore;
	Params.DrawDebugType = DrawDebugType;
	Params.DrawDebugTime = DrawDebugTime;
	Params.SimFrequency = SimFrequency;
	Params.OverrideGravityZ = OverrideGravityZ;
	Params.ObjectTypes = ObjectTypes; // Object trace
	Params.bTraceWithChannel = false;

	// Do the trace
	FPredictProjectilePathResult PredictResult;
	bool bHit = PredictProjectilePath(WorldContextObject, Params, PredictResult);

	// Fill in results.
	OutHit = PredictResult.HitResult;
	OutLastTraceDestination = PredictResult.LastTraceDestination.Location;
	OutPathPositions.Empty(PredictResult.PathData.Num());
	for (const FPredictProjectilePathPointData& PathPoint : PredictResult.PathData)
	{
		OutPathPositions.Add(PathPoint.Location);
	}
	return bHit;
	*/

	GEngine->AddOnScreenDebugMessage(-1, TrajectoryConfig.DelayRecalculateTrajectory, FColor::Green, "Recalculate trajectory");
	// CAS TODO:
	// trajectory var = new trajectory
}

void AShooterWeapon_Grenade::OnHoldRelease()
{
		
	// CAS TODO:
	// Hide trajectory
	// Throw grenade anim
	// Grenade follow trajectory
	// Reduce uses
	
	if(bCanShoot) FireWeapon();
	OnHoldCancel();
}

void AShooterWeapon_Grenade::OnHoldCancel()
{
	
	// CAS TODO:
	// Hide trajectory
	// Back to idle anim
	
	bCanShoot = false;
	if(!IsValid(GetPawnOwner()) ||
		!GetPawnOwner()->GetWorldTimerManager().IsTimerActive(RecalculateTrajectoryHandle)) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Hide trajectory");
	GetPawnOwner()->GetWorldTimerManager().ClearTimer(RecalculateTrajectoryHandle);
}
