﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Ignore/ItemFragment_LineTraceFilterByIgnoreTag.h"

#include "GameplayTagAssetInterface.h"

class IGameplayTagAssetInterface;

void UItemFragment_LineTraceFilterByIgnoreTag::OnInteract()
{
    AActor* DetectedActor;
    TArray<AActor*> TempIgnoreActors;
    for (auto& Actor : IgnoreActors)
    {
        TempIgnoreActors.Add(Actor.Get());
    }

    if (CheckFrontActorTagMatch(TileDistance,DetectedActor, GetFunctionTag(), TempIgnoreActors))
    {
        if (HasIgnoreTag(DetectedActor))
        {
            OnInteract();
            return;
        }
        OnInteractSuccess(DetectedActor);
    }
    IgnoreActors.Reset();
}

bool UItemFragment_LineTraceFilterByIgnoreTag::HasIgnoreTag(AActor* InActor)
{
    if (!InActor)
    {
        return false;
    }

    if (const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(InActor))
    {
        FGameplayTagContainer TagContainer;
        Interface->GetOwnedGameplayTags(TagContainer);

        if (TagContainer.HasTagExact(IgnoreTag))
        {
            IgnoreActors.Add(InActor);
            return true;
        }
    }

    return false;
}
