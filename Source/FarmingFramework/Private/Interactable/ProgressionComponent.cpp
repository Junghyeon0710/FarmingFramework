// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Interactable\ProgressionComponent.h"

UProgressionComponent::UProgressionComponent()
{

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UProgressionComponent::Interact()
{

	if(MeshList.IsEmpty())
	{
		return;
	}

	AActor* Actor = GetOwner();
	check(Actor);
	USkeletalMeshComponent* SkeletalMeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>();
	
	if(SkeletalMeshComponent)
	{
		NewStaticMeshComp = SkeletalMeshComponent;
	}
	else
	{
		NewStaticMeshComp = NewObject<USkeletalMeshComponent>(GetOwner(),USkeletalMeshComponent::StaticClass());
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
			NewStaticMeshComp->SetSkeletalMesh(MeshList[ProgressState].Mesh);
			ProgressState++;
		}

	}),MeshList[ProgressState].TransitionTime,true);
	
}



