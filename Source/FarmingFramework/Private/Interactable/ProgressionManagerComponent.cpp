// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/ProgressionManagerComponent.h"

UProgressionManagerComponent::UProgressionManagerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

float UProgressionManagerComponent::Interact()
{
	float Delay;
	int32 Stage;
	FarmingProgress(Delay,Stage);
	if(MeshList.IsEmpty())
	{
		return 0;
	}

	AActor* Actor = GetOwner();
	check(Actor);
	UStaticMeshComponent* StaticMesh = Actor->FindComponentByClass<UStaticMeshComponent>();
	
	if(StaticMesh)
	{
		NewStaticMeshComp = StaticMesh;
	}
	else
	{
		NewStaticMeshComp = NewObject<UStaticMeshComponent>(GetOwner(),UStaticMeshComponent::StaticClass());
	//	NewStaticMeshComp->SetupAttachment(Actor->GetRootComponent());
		NewStaticMeshComp->AttachToComponent(Actor->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		NewStaticMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		//GetOwner()->AddInstanceComponent(NewStaticMeshComp);
		NewStaticMeshComp->RegisterComponent();
	
	}

	
	GetWorld()->GetTimerManager().SetTimer(ProgressTimer,FTimerDelegate::CreateWeakLambda(this,[this]()
	{
		if(!MeshList.IsValidIndex(ProgressState))
		{
			GetWorld()->GetTimerManager().PauseTimer(ProgressTimer);
			GetWorld()->GetTimerManager().ClearTimer(ProgressTimer);
			//return 0;
			
		}
		else
		{
			NewStaticMeshComp->SetStaticMesh(MeshList[ProgressState].Mesh);
			ProgressState++;
		}

	}),MeshList[ProgressState].TransitionTime,true);

	
	return Delay;

	
}

void UProgressionManagerComponent::SetProgressionState(float Progression)
{
}

void UProgressionManagerComponent::FarmingProgress(float& OutDelay, int32& Stage)
{
}

void UProgressionManagerComponent::SwitchStage()
{
}

void UProgressionManagerComponent::SetReady()
{
}


