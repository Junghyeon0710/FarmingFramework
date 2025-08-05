// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

    UFUNCTION(BlueprintNativeEvent, Category="Inventory")
    void Highlight();

    UFUNCTION(BlueprintNativeEvent, Category="Inventory")
    void Unhighlight();
};
