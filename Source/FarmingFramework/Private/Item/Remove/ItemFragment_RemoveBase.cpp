// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_RemoveBase.h"

#include "Interface/FarmInteractableInterface.h"

bool UItemFragment_RemoveBase::OnInteract()
{
    AActor* DetectedActor;
	if(CheckFrontActorTagMatch(TileDistance,DetectedActor, GetFunctionTag()))
	{
        if (IFarmInteractableInterface* FarmInteractableInterface = Cast<IFarmInteractableInterface>(DetectedActor))
        {
            PlayMontage();
            FarmInteractableInterface->Interact(GetOwner());
            return true;
        }
	}
    return false;
}

void UItemFragment_RemoveBase::OnInteractWithTag(FGameplayTag InFunctionTag)
{
	if(InFunctionTag.MatchesTagExact(FunctionTag))
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());

	}
	else
	{

	}
}

void UItemFragment_RemoveBase::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		if(IsValid(DetectedActor))
		{
			DetectedActor->Destroy();
		}
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());
	}
	else
	{

	}
}
