// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_GroundFlatten.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Ground_Flatten, "Intreact.Ground.Flatten");

void UItemFragment_GroundFlatten::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		// 애니메이션

		//땅 고르기 행동
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());

	}
}

FGameplayTag UItemFragment_GroundFlatten::GetFunctionTag() const
{
	return Intreact_Ground_Flatten;
}


