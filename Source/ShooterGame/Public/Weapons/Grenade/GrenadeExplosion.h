// victorcas

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeExplosion.generated.h"

UENUM()
enum class EGrenadeDamageType
{
	ENormal,
	ELightning,
	EMax,
};

USTRUCT(BlueprintType)
struct FGrenadeExplosionData
{
	GENERATED_USTRUCT_BODY()

	/** Base damage type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGrenadeDamageType GrenadeDamageType = EGrenadeDamageType::ENormal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionDamage = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 500.0f;
};

UCLASS()
class SHOOTERGAME_API AGrenadeExplosion : public AActor
{
	GENERATED_BODY()

public:
	AGrenadeExplosion();

	UFUNCTION()
	void Init(APawn* NewOwner);
	
	/** grenade explosion config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	FGrenadeExplosionData GrenadeExplosionData;
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	APawn* PawnOwner;
};
