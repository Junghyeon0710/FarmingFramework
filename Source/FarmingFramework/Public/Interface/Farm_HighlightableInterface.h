// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Farm_HighlightableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFarm_HighlightableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class FARMINGFRAMEWORK_API IFarm_HighlightableInterface
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, Category="Highlight")
    void Highlight();

    UFUNCTION(BlueprintNativeEvent, Category="Highlight")
    void Unhighlight();

    UFUNCTION(BlueprintNativeEvent, Category="Highlight")
    const FGameplayTagContainer GetHighlightableTags() const;

    // virtual bool IsSameHighlightableTag(const AActor& Other) const
    // {
    //     const IFarm_HighlightableInterface* HighlightableInterface = Cast<const IFarm_HighlightableInterface>(&Other);
    //     return HighlightableInterface ? HighlightableInterface->GetHighlightableTags().HasAnyExact(GetHighlightableTags()) : false;
    // }

};
