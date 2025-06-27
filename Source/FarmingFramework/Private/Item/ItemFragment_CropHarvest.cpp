// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Item\ItemFragment_CropHarvest.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Crop_Harvest, "Interact.Crop.Harvest");


void UItemFragment_CropHarvest::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		// 애니메이션

		// 수확

		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());
	}
}

FGameplayTag UItemFragment_CropHarvest::GetFunctionTag() const
{
	return Interact_Crop_Harvest;
}
