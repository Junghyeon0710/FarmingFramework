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


