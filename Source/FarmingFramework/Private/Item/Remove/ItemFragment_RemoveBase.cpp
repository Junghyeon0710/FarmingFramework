// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Remove/ItemFragment_RemoveBase.h"

void UItemFragment_RemoveBase::OnInteract()
{
	if(CheckFrontActorTagMatch(500.f,GetFunctionTag()))
	{
		//애니메이션 재생

		// 나무 제거
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());
	}
}

void UItemFragment_RemoveBase::OnInteractWithTag(FGameplayTag InFunctionTag)
{
	if(InFunctionTag.MatchesTagExact(FunctionTag))
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());
		
	}
	else
	{
		
	}
}

void UItemFragment_RemoveBase::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		if(IsValid(DetectedActor))
		{
			DetectedActor->Destroy();
		}
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());
	}
	else
	{
		
	}
}
