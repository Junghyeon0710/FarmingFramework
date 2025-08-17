// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FarmSpawnDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FSpawnClassData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftClassPtr<AActor>> ClassRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Probability = -1.0f;
};

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSpawnClassData SpawnClassData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnRatePerFarmSize = 0.1f;
};

UCLASS()
class FARMINGFRAMEWORK_API UFarmSpawnDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSpawnData> SpawnData;
};
