// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TileItemBase.h"

ATileItemBase::ATileItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileItemBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}


