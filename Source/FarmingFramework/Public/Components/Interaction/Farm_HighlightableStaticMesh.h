// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/Farm_HighlightableInterface.h"
#include "Farm_HighlightableStaticMesh.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UFarm_HighlightableStaticMesh : public UStaticMeshComponent, public IFarm_HighlightableInterface
{
    GENERATED_BODY()

public:
    virtual void Highlight_Implementation() override;
    virtual void Unhighlight_Implementation() override;

private:

    UPROPERTY(EditAnywhere, Category="Inventory")
    TObjectPtr<UMaterialInterface> HighlightMaterial;
};
