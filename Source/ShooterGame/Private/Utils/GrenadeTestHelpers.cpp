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

UActorComponent* GrenadeTestHelpers::GetValidatedComponentByClassWithTag(
	AActor* Target, const TSubclassOf<UActorComponent> ComponentClass, FName Tag, GTestEObjectIsValid& OutResult
)
{
	if (!IsValid(Target))
	{
		OutResult = GTestEObjectIsValid::NotValid;
		return nullptr;
	}

	TArray<UActorComponent*> ComponentsWithTag = Target->GetComponentsByTag(ComponentClass, Tag);
	if(ComponentsWithTag.Num() <= 0)
	{
		OutResult = GTestEObjectIsValid::NotValid;
		return nullptr;
	}
	
	UActorComponent* Component = ComponentsWithTag[0];
	if(!IsValid(Component))
	{
		OutResult = GTestEObjectIsValid::NotValid;
		return nullptr;
	}
	
	OutResult = GTestEObjectIsValid::Valid;
	return Component;
}

bool GrenadeTestHelpers::ExistsWallBetweenActors(AActor* OriginActor, AActor* TargetActor, ECollisionChannel TraceChannelToCheck)
{
	if(!IsValid(OriginActor)) return false;
	if(!IsValid(TargetActor)) return false;
	
	const TArray<AActor*>& ActorsToIgnore{OriginActor, TargetActor};

	FHitResult HitResult;
	return UKismetSystemLibrary::LineTraceSingle(OriginActor, OriginActor->GetActorLocation(), TargetActor->GetActorLocation(),
		UEngineTypes::ConvertToTraceType(TraceChannelToCheck),
		true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
}
