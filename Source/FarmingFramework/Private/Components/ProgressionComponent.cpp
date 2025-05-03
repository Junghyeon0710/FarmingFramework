// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Components/ProgressionComponent.h"

#include "Interface/ProgressionStateInterface.h"

UProgressionComponent::UProgressionComponent()
{

	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UProgressionComponent::BeginPlay()
{
	Super::BeginPlay();

	if(IProgressionStateInterface* Interface = Cast<IProgressionStateInterface>(this); Interface && !MeshList.IsEmpty())
	{
		TArray<FGameplayTag> Tags;
		for(const auto& List : MeshList)
		{
			Tags.Add(List.ProgressionState);
		}
		RegisterInitTags(Tags);
	}
}


void UProgressionComponent::Interact()
{
	if(MeshList.IsEmpty())
	{
		return;
	}

	if(!MeshList.IsValidIndex(ProgressState))
	{
		return;
	}
	

	AActor* Owner = GetOwner();
	check(Owner);
	USkeletalMeshComponent* SkeletalMeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
	
	if(SkeletalMeshComponent)
	{
		NewStaticMeshComp = SkeletalMeshComponent;
	}
	else
	{
		NewStaticMeshComp = NewObject<USkeletalMeshComponent>(GetOwner(),USkeletalMeshComponent::StaticClass());
		NewStaticMeshComp->AttachToComponent(Owner->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		NewStaticMeshComp->CreationMethod = EComponentCreationMethod::Instance;
		NewStaticMeshComp->RegisterComponent();
	}

	
	if(ProgressState != 0)
	{
		if(IProgressionStateInterface* Interface = Cast<IProgressionStateInterface>(this))
		{
			if(!Interface->ContinueInitStateChain(GetCurrentState(),GetDesiredState()))
			{
				InitState++;
				return;
			}
		}
	}

	
	NewStaticMeshComp->SetSkeletalMesh(MeshList[ProgressState].Mesh);
	
	
	GetWorld()->GetTimerManager().SetTimer(
	ProgressTimer,
	this,
	&ThisClass::Interact,
	MeshList[ProgressState].TransitionTime,
	false
	);
	ProgressState++;
}

FGameplayTag UProgressionComponent::GetCurrentState()
{
	if(!InitTags.IsValidIndex(InitState))
	{
		return FGameplayTag();
	}
	return InitTags[InitState];
}

FGameplayTag UProgressionComponent::GetDesiredState()
{
	if(!InitTags.IsValidIndex(InitState+1))
	{
		return FGameplayTag();
	}
	return InitTags[InitState+1];
}

void UProgressionComponent::RegisterInitTags(const TArray<FGameplayTag>& InInitTags)
{
	if(!InitTags.IsEmpty())
	{
		UnRegisterInitTags();
	}
					
	InitTags = InInitTags;
}

void UProgressionComponent::UnRegisterInitTags()
{
	InitTags.Empty();
}




