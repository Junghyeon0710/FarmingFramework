// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmTaggedActor.h"
#include "FarmPlotActor.generated.h"

class UBoxComponent;
class ANavMeshBoundsVolume;

UCLASS()
class FARMINGFRAMEWORK_API AFarmPlotActor : public AFarmTaggedActor
{
    GENERATED_BODY()

public:
    AFarmPlotActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
private:

    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;

    UPROPERTY(VisibleAnywhere, Category = "Navigation")
    TObjectPtr<UBoxComponent> NavBoxBoundVolume;
};
