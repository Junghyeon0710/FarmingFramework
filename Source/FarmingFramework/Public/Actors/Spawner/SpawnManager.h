// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ANavMeshBoundsVolume;

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<AActor> ClassRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnRatePerFarmSize = 0.1f; // 농작지 크기 비례 비율
};

class ANavigationData;

UCLASS()
class FARMINGFRAMEWORK_API ASpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ASpawnManager();

protected:
    virtual void BeginPlay() override;

private:

    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavigationData> NavigationData;

    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;

    FVector GetNavVolumeSize() const;

private:
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<FSpawnData> SpawnData;

    int32 CalculateSpawnCountByFarmSizePercentage(float SpawnRatePercent) const;
};
