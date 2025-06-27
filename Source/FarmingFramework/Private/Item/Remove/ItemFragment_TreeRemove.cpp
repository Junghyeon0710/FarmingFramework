// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_TreeRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Remove_Tree, "Interact.Remove.Tree");


FGameplayTag UItemFragment_TreeRemove::GetFunctionTag() const
{
	return Interact_Remove_Tree;
}
