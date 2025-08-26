// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmSpawnManager.h"
#include "SeasonWeatherData.h"
#include "ObstacleSpawnManager.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API AObstacleSpawnManager : public AFarmSpawnManager
{
    GENERATED_BODY()

public:
    AObstacleSpawnManager();

protected:
    virtual void BeginPlay() override;

    virtual bool CanSpawn(const FSpawnData& InSpawnData) const override;

private:

    TSet<EWeatherType> WeatherTypes;
};
