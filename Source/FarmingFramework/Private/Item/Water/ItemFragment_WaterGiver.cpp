// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Water/ItemFragment_WaterGiver.h"
#include "NativeGameplayTags.h"
#include "Actors/InteractableActors/FarmInteractableActor.h"
#include "GameplayTagAssetInterface.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Giver_Water, "Interact.Giver.Water");



void UItemFragment_WaterGiver::OnInteractSuccess(AActor* DetectedActor)
{
    if (AFarmInteractableActor* InteractableActor = Cast<AFarmInteractableActor>(DetectedActor))
    {
        PlayMontage();
        InteractableActor->Interact();
        InteractableActor->AddStaticGameplayTag(IgnoreTag);
    }
}

FGameplayTag UItemFragment_WaterGiver::GetFunctionTag() const
{
	return Interact_Giver_Water;
}

