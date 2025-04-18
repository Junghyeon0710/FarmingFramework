// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProgressionComponentManager.h"

UProgressionComponentManager::UProgressionComponentManager()
{
}

UProgressionComponentManager* UProgressionComponentManager::GetForActor(const AActor* Actor, bool bOnlyGameWorlds)
{
	if (Actor)
	{
		if (const UWorld* ReceiverWorld = Actor->GetWorld())
		{
			if (bOnlyGameWorlds && (!ReceiverWorld->IsGameWorld() || ReceiverWorld->IsPreviewWorld()))
			{
				return nullptr;
			}

			return UGameInstance::GetSubsystem<UProgressionComponentManager>(ReceiverWorld->GetGameInstance());
		}
	}

	return nullptr;
}

void UProgressionComponentManager::RegisterInitTags(const TArray<FGameplayTag>& InInitTags)
{
	if(!InitTags.IsEmpty())
	{
		UnRegisterInitTags();
	}

	InitTags = InInitTags;
}

void UProgressionComponentManager::UnRegisterInitTags()
{
	InitTags.Empty();
}

FGameplayTag UProgressionComponentManager::GetCurrentState()
{
	if(!InitTags.IsValidIndex(InitState))
	{
		return FGameplayTag();
	}
	return InitTags[InitState];
}

FGameplayTag UProgressionComponentManager::GetDesiredState()
{
	if(!InitTags.IsValidIndex(InitState+1))
	{
		return FGameplayTag();
	}
	return InitTags[InitState+1];
}

void UProgressionComponentManager::NextInitState()
{
	InitState++;
}
