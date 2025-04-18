// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/ProgressionStateInterface.h"
#include "Components/ProgressionComponentManager.h"

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

void IProgressionStateInterface::RegisterInitTags(const TArray<FGameplayTag>& Tags)
{
	AActor* MyActor = GetOwningActor();
	UProgressionComponentManager* Manager = UProgressionComponentManager::GetForActor(MyActor);

	if(ensure(Manager && MyActor))
	{
		Manager->RegisterInitTags(Tags);
	}
}

bool IProgressionStateInterface::ContinueInitStateChain()
{
	const AActor* MyActor = GetOwningActor();
	UProgressionComponentManager* Manager = UProgressionComponentManager::GetForActor(MyActor);

	if(!Manager || !MyActor)
	{
		return false;
	}

	const FGameplayTag CurrentState = Manager->GetCurrentState();
	const FGameplayTag DesiredState = Manager->GetDesiredState();

	if (!CurrentState.IsValid() || !DesiredState.IsValid())
	{
		return false;
	}

	if (CurrentState == DesiredState)
	{
		return false;
	}

	
	if(CanChangeInitState(Manager,CurrentState,DesiredState))
	{
		HandleChangeInitState(Manager,CurrentState,DesiredState);
		Manager->NextInitState();
		return true;
	}
	

	HandleFailedInitStateChange(Manager,CurrentState,DesiredState);
	return false;
	
}

// Add default functionality here for any IProgressionStateInterface functions that are not pure virtual.
void IProgressionStateInterface::BindOnActorInitStateChanged()
{
	AActor* MyActor = GetOwningActor();
	UProgressionComponentManager* Manager = UProgressionComponentManager::GetForActor(MyActor);

	if(ensure(Manager && MyActor))
	{
		
	}
}
