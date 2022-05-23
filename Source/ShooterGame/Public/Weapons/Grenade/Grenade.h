// victorcas

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

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

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
};
