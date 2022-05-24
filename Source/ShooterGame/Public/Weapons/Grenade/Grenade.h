// victorcas

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

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
struct FGrenadeData
{
	GENERATED_USTRUCT_BODY()

	/** Base grenade type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGrenadeType GrenadeType = EGrenadeType::EBouncing;

	/** Setting this time to 0 means it will not explode automatically after a delay (useful for mines or similar grenades) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionDelay = 3.0f;

	/** Setting this time to 0 means the grenade will never be destroyed after explosion (useful for flares or similar grenades) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestroyGrenadeAfterExplosionDelay = .01f;
};

/**
 * This class represents the grenade itself when has been already thrown
 * It should have all effects like damage, vfx, etc
 * It should also follow a trajectory, starting at the begin play (this is spawned when thrown)
 */
UCLASS()
class SHOOTERGAME_API AGrenade : public AActor
{
	GENERATED_BODY()

public:
	AGrenade();

	UFUNCTION()
	void Init(FVector NewImpulseVector, APawn* NewOwner);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGrenadeExplosion> GrenadeExplosionClassToSpawn;
	
	/** grenade config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FGrenadeData GrenadeData;

	UFUNCTION(BlueprintImplementableEvent)
	void ExplodeGrenade_BP();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

private:
	FVector ImpulseVector = FVector::ZeroVector;

	FTimerHandle DelayGrenadeExplosionHandle;
	FTimerHandle DelayDestroyGrenadeHandle;

	UFUNCTION()
	void SpawnGrenadeExplosion();
	
	UFUNCTION()
	void ExplodeGrenade();

	UFUNCTION()
	void DestroyGrenade();
	
	UPROPERTY()
	APawn* PawnOwner;
};
