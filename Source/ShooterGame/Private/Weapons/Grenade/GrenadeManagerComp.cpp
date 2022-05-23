// Copyright victorcas

#include "Grenade/GrenadeManagerComp.h"

UGrenadeManagerComp::UGrenadeManagerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrenadeManagerComp::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UGrenadeManagerComp::TickComponent(float DeltaTime, ELevelTick TickType,
										FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UGrenadeManagerComp::IsGrenadeEquipped()
{
	return IsValid(GetGrenadeEquippedOrNull());
}

AShooterWeapon_Grenade* UGrenadeManagerComp::GetGrenadeEquippedOrNull()
{
	AShooterWeapon* Weapon = AShooterCharacter::GetWeapon_Static(GetOwner());
	if(!IsValid(Weapon)) return nullptr;
	return Cast<AShooterWeapon_Grenade>(Weapon);
}

void UGrenadeManagerComp::CancelCurrentThrow()
{
	AShooterWeapon_Grenade* Grenade = GetGrenadeEquippedOrNull();
	if(!IsValid(Grenade)) return;
	Grenade->OnHoldCancel();
}

void UGrenadeManagerComp::EquipWeapon()
{
	if(!IsValid(PreviousWeapon_Cached)) return;
	
	AShooterWeapon_Grenade* PreviousGrenade_Cached = GetGrenadeEquippedOrNull();

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Equip weapon");
	AShooterCharacter::EquipWeapon_Static(GetOwner(), PreviousWeapon_Cached);

	if(IsValid(PreviousGrenade_Cached))
	{
		PreviousGrenade_Cached->Destroy();
	}
}

void UGrenadeManagerComp::EquipNextGrenade()
{
	if(ArrayGrenadeClasses.Num() <= 0) return;
	AShooterWeapon_Grenade* GrenadeToEquip = nullptr;
	AShooterWeapon_Grenade* PreviousGrenade_Cached = nullptr;
	
	if(!IsGrenadeEquipped())
	{
		for(const auto G: ArrayGrenadeClasses)
		{
			GrenadeToEquip = SpawnGrenadeWeaponOfClass(G);
			if(IsValid(GrenadeToEquip))
			{
				PreviousWeapon_Cached = AShooterCharacter::GetWeapon_Static(GetOwner());
				break;
			}
		}
	}
	else
	{
		if(ArrayGrenadeClasses.Num() == 1) return;
		PreviousGrenade_Cached = GetGrenadeEquippedOrNull();
		
		const TSubclassOf<class AShooterWeapon_Grenade> EquippedGrenadeClass = GetGrenadeEquippedOrNull()->GetClass();
		const int StartPos = ArrayGrenadeClasses.IndexOfByKey(EquippedGrenadeClass);

		for(int i = StartPos; ;)
		{
			++i; if(i >= ArrayGrenadeClasses.Num()) i = 0;
			if(ArrayGrenadeClasses[i] == EquippedGrenadeClass) break;
			GrenadeToEquip = SpawnGrenadeWeaponOfClass(ArrayGrenadeClasses[i]);
			if(IsValid(GrenadeToEquip)) break;
		}
	}
	if(IsValid(GrenadeToEquip))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Equip grenade");
		AShooterCharacter::EquipWeapon_Static(GetOwner(), GrenadeToEquip);

		if(IsValid(PreviousGrenade_Cached))
		{
			PreviousGrenade_Cached->Destroy();
		}
	}
}

AShooterWeapon_Grenade* UGrenadeManagerComp::SpawnGrenadeWeaponOfClass(TSubclassOf<AShooterWeapon_Grenade> GrenadeClass)
{
	if(!IsValid(GrenadeClass)) return nullptr;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<AShooterWeapon_Grenade>(GrenadeClass, SpawnInfo);
}

void UGrenadeManagerComp::HoldGrenadeButton()
{
	if(IsGrenadeEquipped())
	{
		if(GetOwner()->GetWorldTimerManager().IsTimerActive(HoldToEquipWeaponHandle)) return;
		GetOwner()->GetWorldTimerManager().SetTimer(HoldToEquipWeaponHandle, this,
																		&UGrenadeManagerComp::ReleaseGrenadeButton,
																		TimeHoldToEquipWeapon, true);
	}
	else
	{
		EquipNextGrenade();
	}
}

void UGrenadeManagerComp::ReleaseGrenadeButton()
{
	if(!GetOwner()->GetWorldTimerManager().IsTimerActive(HoldToEquipWeaponHandle)) return;
	if(GetOwner()->GetWorldTimerManager().GetTimerElapsed(HoldToEquipWeaponHandle) >= TimeHoldToEquipWeapon)
	{
		EquipWeapon();
	}
	else
	{
		EquipNextGrenade();
	}
	GetOwner()->GetWorldTimerManager().ClearTimer(HoldToEquipWeaponHandle);
}
