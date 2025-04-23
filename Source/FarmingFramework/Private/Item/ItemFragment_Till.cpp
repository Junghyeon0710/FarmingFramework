// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Item\ItemFragment_Till.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Intreact_Till, "Intreact.till");

void UItemFragment_Till::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		//애니메이션

		//개간 기능
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());

	}
}

FGameplayTag UItemFragment_Till::GetFunctionTag() const
{
	return Intreact_Till;
}

