// Copyright victorcas

#pragma once
#include "CoreMinimal.h"
#include "ShooterWeapon_Grenade.h"
#include "Components/ActorComponent.h"
#include "GrenadeManagerComp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
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

	UPROPERTY(EditDefaultsOnly, Category="Grenade Manager")
	FName TagComponentSpawnPoint = "SpawnPoint";
	
	UPROPERTY(EditDefaultsOnly, Category="Grenade Manager")
	float TimeHoldToEquipWeapon = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Grenade Manager")
	TArray<TSubclassOf<class AShooterWeapon_Grenade>> ArrayGrenadeClasses;

	UFUNCTION(BlueprintCallable)
	void CancelCurrentThrow();

	UPROPERTY(EditDefaultsOnly, Category="Grenade Manager")
	bool bCanShowTrajectory = true;
	
	UFUNCTION()
	void ShowTrajectory(bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowTrajectory_BP(bool bShow);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector GetImpulseVector(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FTransform GetGrenadeSpawnTransformPoint(AActor* TargetActor);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	void EquipWeapon();
	void EquipNextGrenade();
	AShooterWeapon_Grenade* SpawnGrenadeWeaponOfClass(TSubclassOf<class AShooterWeapon_Grenade> GrenadeClass);
	
	FTimerHandle HoldToEquipWeaponHandle;
	
	UPROPERTY()
	AShooterWeapon* PreviousWeapon_Cached;
	
};
