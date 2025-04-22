// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_GrassRemove.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Remove_Grass, "Intreact.Remove.Grass");

FGameplayTag UItemFragment_GrassRemove::GetFunctionTag() const
{
	return Intreact_Remove_Grass;
}
