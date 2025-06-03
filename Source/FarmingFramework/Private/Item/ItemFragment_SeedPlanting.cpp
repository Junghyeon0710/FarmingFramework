// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_SeedPlanting.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Planting_Seed, "Intreact.Planting.Seed");


void UItemFragment_SeedPlanting::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if (InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		// 애니메이션

		// 수확

		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetFunctionTag().ToString());
	}
	
}

FGameplayTag UItemFragment_SeedPlanting::GetFunctionTag() const
{
	return Intreact_Planting_Seed;
}
