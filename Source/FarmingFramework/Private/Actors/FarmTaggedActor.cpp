// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Actors\FarmTaggedActor.h"

AFarmTaggedActor::AFarmTaggedActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFarmTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}

void AFarmTaggedActor::AddStaticGameplayTag(const FGameplayTag& NewTag)
{
	if (!NewTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddStaticGameplayTag: Invalid tag"));
		return;
	}

	if (!StaticGameplayTags.HasTag(NewTag))
	{
		StaticGameplayTags.AddTag(NewTag);
		UE_LOG(LogTemp, Log, TEXT("Tag %s added to StaticGameplayTags"), *NewTag.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("Tag %s already exists"), *NewTag.ToString());
	}
}


