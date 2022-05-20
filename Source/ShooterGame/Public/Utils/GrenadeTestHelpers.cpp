#include "GrenadeTestHelpers.h"

UActorComponent* GrenadeTestHelpers::GetValidatedComponentByClass(
	AActor* Target, const TSubclassOf<UActorComponent> ComponentClass, GTestEObjectIsValid& OutResult
)
{
	if (!IsValid(Target))
	{
		OutResult = GTestEObjectIsValid::NotValid;
		return nullptr;
	}
	
	UActorComponent* Component = Target->FindComponentByClass(ComponentClass);
	if(!IsValid(Component))
	{
		OutResult = GTestEObjectIsValid::NotValid;
		return nullptr;
	}
	
	OutResult = GTestEObjectIsValid::Valid;
	return Component;
}
