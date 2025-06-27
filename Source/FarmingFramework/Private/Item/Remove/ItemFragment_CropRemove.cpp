// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_CropRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Remove_Crop, "Interact.Remove.Crop");

FGameplayTag UItemFragment_CropRemove::GetFunctionTag() const
{
	return Interact_Remove_Crop;
}

