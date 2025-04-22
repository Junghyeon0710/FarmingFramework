// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_TreeRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Remove_Tree, "Intreact.Remove.Tree");


FGameplayTag UItemFragment_TreeRemove::GetFunctionTag() const
{
	return Intreact_Remove_Tree;
}
