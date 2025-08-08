// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Interaction/Farm_HighlightableStaticMesh.h"

#include "GameplayTagAssetInterface.h"

void UFarm_HighlightableStaticMesh::Highlight_Implementation()
{
    check(HighlightMaterial);
    SetOverlayMaterial(HighlightMaterial);
}

void UFarm_HighlightableStaticMesh::Unhighlight_Implementation()
{
    SetOverlayMaterial(nullptr);
}

const FGameplayTagContainer UFarm_HighlightableStaticMesh::GetHighlightableTags_Implementation() const
{
    if (!GetOwner())
    {
        return FGameplayTagContainer();
    }

    if (IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(GetOwner()))
    {
        FGameplayTagContainer FoundTags;
        GameplayTagAssetInterface->GetOwnedGameplayTags(FoundTags);
        return FoundTags;
    }

    return FGameplayTagContainer();
}
