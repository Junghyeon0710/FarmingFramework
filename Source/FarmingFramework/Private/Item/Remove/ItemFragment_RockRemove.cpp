// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_RockRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Remove_Rock, "Interact.Remove.Rock");

FGameplayTag UItemFragment_RockRemove::GetFunctionTag() const
{
	return Interact_Remove_Rock;
}
