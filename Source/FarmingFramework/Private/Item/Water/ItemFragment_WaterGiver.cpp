// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Water/ItemFragment_WaterGiver.h"
#include "NativeGameplayTags.h"
#include "Actors/InteractableActors/FarmInteractableActor.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Giver_Water, "Interact.Giver.Water");

void UItemFragment_WaterGiver::OnInteract()
{
	AActor* DetectedActor;
	if (CheckFrontActorTagMatch(TileDistance,DetectedActor, GetFunctionTag()))
	{
		if (AFarmInteractableActor* InteractableActor = Cast<AFarmInteractableActor>(DetectedActor))
		{
			InteractableActor->Interact();
		}
	}
}

FGameplayTag UItemFragment_WaterGiver::GetFunctionTag() const
{
	return Interact_Giver_Water;
}
