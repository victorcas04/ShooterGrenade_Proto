// Copyright victorcas

#pragma once
#include "CoreMinimal.h"
#include "ShooterWeapon_Grenade.h"
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsGrenadeEquipped();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AShooterWeapon_Grenade* GetGrenadeEquippedOrNull();
	
	UFUNCTION()
	void HoldGrenadeButton();

	UFUNCTION()
	void ReleaseGrenadeButton();

	UPROPERTY(EditDefaultsOnly, Category="Hold Grenade Input")
	float TimeHoldToEquipWeapon = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Grenades")
	TArray<TSubclassOf<class AShooterWeapon_Grenade>> ArrayGrenadeClasses;

	UFUNCTION(BlueprintCallable)
	void CancelCurrentThrow();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void EquipWeapon();
	void EquipNextGrenade();
	AShooterWeapon_Grenade* SpawnGrenadeWeaponOfClass(TSubclassOf<class AShooterWeapon_Grenade> GrenadeClass);

	AShooterWeapon* PreviousWeapon_Cached;
	FTimerHandle HoldToEquipWeaponHandle;
	
};
