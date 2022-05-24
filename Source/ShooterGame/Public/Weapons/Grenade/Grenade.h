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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionDelay = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionDamage = 50.0f;
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
	USkeletalMeshComponent* GrenadeMesh;

	/** grenade config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FGrenadeData GrenadeData;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

private:
	FVector ImpulseVector = FVector::ZeroVector;

	UPROPERTY()
	APawn* PawnOwner;
};
