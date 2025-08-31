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
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#if WITH_EDITOR
    virtual void PostEditMove(bool bFinished) override;
#endif
private:

    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;

    UPROPERTY(VisibleAnywhere, Category = "Navigation")
    TObjectPtr<UBoxComponent> NavBoxBoundVolume;

    /**
     * NavMeshBoundsVolume 갱신 함수
     * 에디터 전용: 이동, 스케일, Brush 변경 후 호출
     */
    void UpdateNavMeshBoundsVolume();
};
