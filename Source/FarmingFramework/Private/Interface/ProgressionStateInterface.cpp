// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ProgressionStateInterface.h"

AActor* IProgressionStateInterface::GetOwningActor() const
{
	// Removing const because most AActor functions can't handle const
	AActor* FoundActor = const_cast<AActor*>(Cast<AActor>(this));

	if (!FoundActor)
	{
		const UActorComponent* FoundComponent = Cast<UActorComponent>(this);
		if (FoundComponent)
		{
			FoundActor = FoundComponent->GetOwner();
		}
	}

	if (ensure(FoundActor))
	{
		// Subclasses must implement this if they are not actors or components
		return FoundActor;
	}

	return nullptr;
}

// void IProgressionStateInterface::RegisterInitTags(const TArray<FGameplayTag>& Tags)
// {
// 	AActor* MyActor = GetOwningActor();
// 	UProgressionComponentManager* Manager = UProgressionComponentManager::GetForActor(MyActor);
//
// 	if(ensure(Manager && MyActor))
// 	{
// 		Manager->RegisterInitTags(Tags);
// 	}
// }

bool IProgressionStateInterface::ContinueInitStateChain(FGameplayTag CurrentState,FGameplayTag DesiredState)
{
	const AActor* MyActor = GetOwningActor();

	if(!MyActor)
	{
		return false;
	}

	if (!CurrentState.IsValid() || !DesiredState.IsValid())
	{
		return false;
	}

	if (CurrentState == DesiredState)
	{
		return false;
	}


	if(CanChangeInitState(CurrentState,DesiredState))
	{
		HandleChangeInitState(CurrentState,DesiredState);

		return true;
	}


	HandleFailedInitStateChange(CurrentState,DesiredState);

	return false;

}

// Add default functionality here for any IProgressionStateInterface functions that are not pure virtual.
void IProgressionStateInterface::BindOnActorInitStateChanged()
{

}
