// Copyright victorcas

#pragma once

#include "ShooterWeapon.h"
#include "ShooterWeapon_Grenade.generated.h"

class AGrenade;

/*
 * We could avoid having this enum with the conversion to EAmmoType in the GetAmmoType function, but this way we avoid
 * possible problems with editor modifications, avoiding setting any Bullet ammo type to any grenade
 */
UENUM()
enum class EGrenadeType
{
	EBouncing,
	ESticky,
	EMax,
};

USTRUCT(BlueprintType)
struct FGrenadeTrajectoryData
{
	GENERATED_USTRUCT_BODY()

	/** base grenade type */
	UPROPERTY(EditDefaultsOnly, Category="Type")
	EGrenadeType GrenadeType = EGrenadeType::EBouncing;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrowForce = 100.0f;
	
	// Projectile radius, used when tracing for collision. If <= 0, a line trace is used instead.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileRadius = 5.f;

	// Object type to use, when checking collisions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>({
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
	});

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;
	
	/**
	 *	Determines size of each sub-step in the simulation (ms).
	 *	10 = +quality
	 *	30 = +performance
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=10.0f, ClampMax=30.0f))
	float SimFrequency = 10.0f;
	
	// Maximum simulation time for the virtual projectile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxSimTime = 3.f;
	
	// The acceleration of the gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Gravity = 980.7f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowTraceDebug = false;
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
	
	virtual EAmmoType GetAmmoType() const override
	{
		if(TrajectoryConfig.GrenadeType == EGrenadeType::EBouncing) return EAmmoType::EBouncingGrenade;
		if(TrajectoryConfig.GrenadeType == EGrenadeType::ESticky) return EAmmoType::EStickyGrenade;
		return EAmmoType::EMax;
	}

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenade> GrenadeClassToSpawn;
	
	/** weapon config */
	UPROPERTY(EditDefaultsOnly, Category=Config)
	FGrenadeTrajectoryData TrajectoryConfig;

	virtual void Destroyed() override;

	//////////////////////////////////////////////////////////////////////////
	// Input

	/** [local + server] start weapon fire */
	virtual void StartFire() override;

	/** [local + server] stop weapon fire */
	virtual void StopFire() override;
	
	/** [all] start weapon reload */
	virtual void StartReload(bool bFromReplication = false) override;

	/** [local + server] interrupt weapon reload */
	virtual void StopReload() override;

	/** [server] performs actual reload */
	virtual void ReloadWeapon() override;
	
	UFUNCTION()
	void OnHoldCancel();
	
protected:

	//////////////////////////////////////////////////////////////////////////
	// Weapon usage
	
	/** [local] weapon specific fire implementation */
	virtual void FireWeapon() override;
	
	/** [local + server] firing started */
	virtual void OnBurstStarted() override;

	/** [local + server] firing finished */
	virtual void OnBurstFinished() override;

private:

	// CAS TODO:
	// trajectory var

	UPROPERTY()
	bool bCanShoot = false;
	
	UPROPERTY()
	float DelayRecalculateTrajectory = .1f;

	UPROPERTY()
	FTimerHandle RecalculateTrajectoryHandle;
	
	UFUNCTION()
	void OnHoldStart();

	UFUNCTION()
	void OnHoldLoop();

	UFUNCTION()
	void OnHoldRelease();
};
