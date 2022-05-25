﻿#pragma once

UENUM()
enum class GTestEObjectIsValid : uint8
{
	Valid,
	NotValid,
};

class GrenadeTestHelpers
{
public:


	/** Searches components array and returns first encountered component of the specified class. */
	UFUNCTION(BlueprintCallable, Category = "Actor", meta = (ComponentClass = "ActorComponent", DeterminesOutputType =
		"ComponentClass", ExpandEnumAsExecs = "OutResult"))
	static UActorComponent* GetValidatedComponentByClass(AActor* Target,
	                                                     TSubclassOf<UActorComponent> ComponentClass,
	                                                     GTestEObjectIsValid& OutResult);

	/** Searches components array and returns first encountered component of the specified class. */
	UFUNCTION(BlueprintCallable, Category = "Actor", meta = (ComponentClass = "ActorComponent", DeterminesOutputType =
		"ComponentClass", ExpandEnumAsExecs = "OutResult"))
	static UActorComponent* GetValidatedComponentByClassWithTag(AActor* Target,
														 TSubclassOf<UActorComponent> ComponentClass,
														 FName Tag,
														 GTestEObjectIsValid& OutResult);
	
	// ECC_GameTraceChannel2 == Projectile
	UFUNCTION(BlueprintCallable, Category = "Trace")
	static bool ExistsWallBetweenActors(AActor* OriginActor, AActor* TargetActor, ECollisionChannel TraceChannelToCheck = ECollisionChannel::ECC_EngineTraceChannel2);
	
};
