// Copyright victorcas

#pragma once

#include "ShooterWeapon.h"
#include "ShooterWeapon_Grenade.generated.h"

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

USTRUCT()
struct FGrenadeWeaponData
{
	GENERATED_USTRUCT_BODY()

	/** base grenade type */
	UPROPERTY(EditDefaultsOnly, Category="Type")
	EGrenadeType GrenadeType = EGrenadeType::EBouncing;
	
	/** base weapon spread (degrees) */
	UPROPERTY(EditDefaultsOnly, Category=Accuracy)
	float WeaponSpread;

	/** targeting spread modifier */
	UPROPERTY(EditDefaultsOnly, Category=Accuracy)
	float TargetingSpreadMod;

	/** continuous firing: spread increment */
	UPROPERTY(EditDefaultsOnly, Category=Accuracy)
	float FiringSpreadIncrement;

	/** continuous firing: max increment */
	UPROPERTY(EditDefaultsOnly, Category=Accuracy)
	float FiringSpreadMax;

	/** weapon range */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	float WeaponRange;

	/** damage amount */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	int32 HitDamage;

	/** type of damage */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	TSubclassOf<UDamageType> DamageType;

	/** hit verification: scale for bounding box of hit actor */
	UPROPERTY(EditDefaultsOnly, Category=HitVerification)
	float ClientSideHitLeeway;

	/** hit verification: threshold for dot product between view direction and hit direction */
	UPROPERTY(EditDefaultsOnly, Category=HitVerification)
	float AllowedViewDotHitDir;

	/** defaults */
	FGrenadeWeaponData()
	{
		GrenadeType = EGrenadeType::EBouncing;
		WeaponSpread = 5.0f;
		TargetingSpreadMod = 0.25f;
		FiringSpreadIncrement = 1.0f;
		FiringSpreadMax = 10.0f;
		WeaponRange = 10000.0f;
		HitDamage = 10;
		DamageType = UDamageType::StaticClass();
		ClientSideHitLeeway = 200.0f;
		AllowedViewDotHitDir = 0.8f;
	}
};

UCLASS(Abstract, Blueprintable)
class AShooterWeapon_Grenade : public AShooterWeapon
{
	GENERATED_UCLASS_BODY()
	
	virtual EAmmoType GetAmmoType() const override
	{
		if(GrenadeConfig.GrenadeType == EGrenadeType::EBouncing) return EAmmoType::EBouncingGrenade;
		if(GrenadeConfig.GrenadeType == EGrenadeType::ESticky) return EAmmoType::EStickyGrenade;
		return EAmmoType::EMax;
	}

	/** weapon config */
	UPROPERTY(EditDefaultsOnly, Category=Config)
	FGrenadeWeaponData GrenadeConfig;

	// TODO:
	// destroy trajectory
	virtual void Destroyed() override;

	//////////////////////////////////////////////////////////////////////////
	// Input

	/** [local + server] start weapon fire */
	// TODO:
	// start holding
	virtual void StartFire() override;

	// TODO:
	// end holding
	/** [local + server] stop weapon fire */
	virtual void StopFire() override;

	// TODO:
	// add function for cancel holding input (right click)
	
	/** [all] start weapon reload */
	virtual void StartReload(bool bFromReplication = false) override;

	/** [local + server] interrupt weapon reload */
	virtual void StopReload() override;

	/** [server] performs actual reload */
	virtual void ReloadWeapon() override;

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
	// TODO:
	// Show trajectory
	// Hold animation
	UFUNCTION()
	void OnHoldStart();

	// TODO:
	// Hide trajectory
	// Throw grenade anim
	// Grenade follow trajectory
	// Reduce uses
	UFUNCTION()
	void OnHoldEnd();

	// TODO:
	// Hide trajectory
	// Back to idle anim
	UFUNCTION()
	void OnHoldCancel();
};
