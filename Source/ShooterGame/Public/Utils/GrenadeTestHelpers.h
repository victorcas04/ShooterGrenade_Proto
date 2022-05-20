#pragma once

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
	
};
