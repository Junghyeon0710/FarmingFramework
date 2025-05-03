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

