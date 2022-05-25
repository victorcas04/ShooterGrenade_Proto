// Copyright victorcas

#pragma once

#include "ShooterWeapon.h"
#include "ShooterWeapon_Grenade.generated.h"

USTRUCT(BlueprintType)
struct FGrenadeTrajectoryData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1.0f, ClampMax=1000.0f))
	float ThrowForce = 1000.0f;
	
	// Projectile radius, used when tracing for collision.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1.0f, ClampMax=100.0f))
	float ProjectileRadius = 10.0f;

	// Trace channel to use when checking collisions.
	// ECC_GameTraceChannel2 == Projectile
	ECollisionChannel TrajectoryChannel = ECollisionChannel::ECC_GameTraceChannel2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ShowTraceDebug = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.01f))
	float DelayRecalculateTrajectory = .1f;
	
	/**
	 *	Determines size of each sub-step in the simulation (ms).
	 *	10 = +quality
	 *	30 = +performance
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=10.0f, ClampMax=30.0f))
	float SimFrequency = 10.0f;
	
	// Maximum simulation time for the virtual projectile.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxSimTime = 3.f;
	
	// The acceleration of the gravity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Gravity = -980.7f;
};

/**
 * This class represents the grenade while holding it
 * Should have only "weapon" related things and the trajectory
 * All grenade effects like what happens when the grenade explodes, should go in the AGrenade class 
 */
UCLASS(Abstract, Blueprintable)
class AShooterWeapon_Grenade : public AShooterWeapon
{
	GENERATED_UCLASS_BODY()
	
	virtual EAmmoType GetAmmoType() const override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenade> GrenadeClassToSpawn;
	
	/** trajectory config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FGrenadeTrajectoryData TrajectoryData;

	virtual void Destroyed() override;

	//////////////////////////////////////////////////////////////////////////
	// Input

	/** [local + server] start weapon fire */
	virtual void StartFire() override;

	/** [local + server] stop weapon fire */
	virtual void StopFire() override;
	
	UFUNCTION()
	void OnHoldCancel();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanThrowGrenade() const;
	
protected:

	//////////////////////////////////////////////////////////////////////////
	// Weapon usage
	
	/** [local] weapon specific fire implementation */
	virtual void FireWeapon() override;

private:

	UPROPERTY()
	bool bCanShoot = false;
	
	UPROPERTY()
	FTimerHandle RecalculateTrajectoryHandle;
	
	UFUNCTION()
	void OnHoldStart();

	UFUNCTION()
	void OnHoldLoop();

	UFUNCTION()
	void OnHoldRelease();
};
