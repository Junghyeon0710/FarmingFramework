// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_CropRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Remove_Crop, "Intreact.Remove.Crop");

FGameplayTag UItemFragment_CropRemove::GetFunctionTag() const
{
	return Intreact_Remove_Crop;
}

