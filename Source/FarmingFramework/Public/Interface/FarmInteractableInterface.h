// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FarmInteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFarmInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class FARMINGFRAMEWORK_API IFarmInteractableInterface
{
    GENERATED_BODY()

public:

    virtual void Interact(AActor* Interactor) = 0;
};
