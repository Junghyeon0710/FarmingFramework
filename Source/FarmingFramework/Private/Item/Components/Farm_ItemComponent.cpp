// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Components/Farm_ItemComponent.h"

#include "Item/FarmItemFragment.h"


UFarm_ItemComponent::UFarm_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UFarmItemFragment* UFarm_ItemComponent::GetFarmItemFragment(TSubclassOf<UFarmItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UFarmItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

void UFarm_ItemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UFarmItemFragment* Fragment : Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnFragmentCreated();
		}
	}
}

void UFarm_ItemComponent::TryUseItem()
{
	for (UFarmItemFragment* Fragment : Fragments)
	{
	    if (!UFarmItemFragment::IsInteractBlocked())
	    {
		    Fragment->OnInteract();
	    }
	}
}

AActor* UFarm_ItemComponent::FindInteractableActorFromItems() const
{
    for (UFarmItemFragment* Fragment : Fragments)
    {
        if (AActor* InteractableActor = Fragment->GetInteractableActor())
        {
            return InteractableActor;
        }
    }

    return nullptr;
}


