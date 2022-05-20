// Copyright victorcas

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrenadeManagerComp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTERGAME_API UGrenadeManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrenadeManagerComp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HoldGrenadeButton();

	UFUNCTION()
	void ReleaseGrenadeButton();

	UPROPERTY(EditDefaultsOnly, Category="Hold Grenade Input")
	float TimeHoldToEquipWeapon = 1.5f;
	
	// CAS TODO:
	// TArray<GrenadeWeapon>
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void EquipWeapon();
	void SwapGrenade();
	
	FTimerHandle HoldToEquipWeaponHandle;
	
};
