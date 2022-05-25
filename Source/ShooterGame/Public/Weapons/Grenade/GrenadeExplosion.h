// victorcas

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeExplosion.generated.h"

USTRUCT(BlueprintType)
struct FGrenadeExplosionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanDamageOwner = true;
	
	/** type of damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionDamage = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 500.0f;

	/** If true -> explosion area persists for a while */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bExplosionPersists = false;
	
	/*
	 * Time before explosion dissapears (this ca be used for incendiary/gas grenades)
	 * If time = 0 -> area persists forever
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bExplosionPersists"))
	float DelayBeforeExplosionEnds = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bExplosionPersists"))
	float DPS = 10.0f;
};

UCLASS()
class SHOOTERGAME_API AGrenadeExplosion : public AActor
{
	GENERATED_BODY()

public:
	AGrenadeExplosion();

	/** initial setup */
	UFUNCTION()
	void Init(APawn* NewOwner, AActor* GrenadeSpawner);
	
	/** grenade explosion config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FGrenadeExplosionData GrenadeExplosionData;

	/** This is here so other team members can add vfx on blueprint */
	UFUNCTION(BlueprintImplementableEvent)
	void ExplosionVFX_BP();

	/** This is here so other team members can add sfx on blueprint */
	UFUNCTION(BlueprintImplementableEvent)
	void ExplosionSFX_BP();
	
	/** This is here so other team members can add functionality on blueprint when explosion damages an actor*/
	UFUNCTION(BlueprintImplementableEvent)
	void ExplosionDamagedActor_BP(AActor* DamagedActor, float DamageApplied);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* TriggerExplosion;
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APawn* PawnOwner;

	UPROPERTY()
	TArray<AActor*> DamageActorsToIgnore;
	
	float CurrentDamage;
	FTimerHandle DelayBeforeExplosionEndsHandle;

	UFUNCTION()
	void OnTriggerExplosionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerExplosionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DestroyGrenadeExplosion();
};
