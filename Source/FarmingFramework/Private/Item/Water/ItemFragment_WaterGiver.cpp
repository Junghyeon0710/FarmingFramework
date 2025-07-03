// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Water/ItemFragment_WaterGiver.h"
#include "NativeGameplayTags.h"
#include "Actors/InteractableActors/FarmInteractableActor.h"
#include "GameplayTagAssetInterface.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Giver_Water, "Interact.Giver.Water");

void UItemFragment_WaterGiver::OnInteract()
{
	AActor* DetectedActor;
	TArray<AActor*> TempIgnoreActors;
	for (auto& Actor : IgnoreActors)
	{
		TempIgnoreActors.Add(Actor.Get());
	}
	
	if (CheckFrontActorTagMatch(TileDistance,DetectedActor, GetFunctionTag(), TempIgnoreActors))
	{
		if (HasIgnoreTag(DetectedActor))
		{
			OnInteract();
			return;
		}
		if (AFarmInteractableActor* InteractableActor = Cast<AFarmInteractableActor>(DetectedActor))
		{
			InteractableActor->Interact();
			InteractableActor->AddStaticGameplayTag(IgnoreTag);
		}
	}
	IgnoreActors.Reset();
}

FGameplayTag UItemFragment_WaterGiver::GetFunctionTag() const
{
	return Interact_Giver_Water;
}

bool UItemFragment_WaterGiver::HasIgnoreTag(AActor* InActor)
{
	if (!InActor)
	{
		return false;
	}

	if (const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(InActor))
	{
		FGameplayTagContainer TagContainer;
		Interface->GetOwnedGameplayTags(TagContainer);

		if (TagContainer.HasTagExact(IgnoreTag))
		{
			IgnoreActors.Add(InActor);
			return true;
		}
	}

	return false;
}


