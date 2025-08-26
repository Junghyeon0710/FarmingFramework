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

    /** AFarmSpawnManager */
    virtual bool CanSpawn(const FSpawnData& InSpawnData) const override;
    virtual void ReSpawn() override;
    /** ~AFarmSpawnManager */

private:
    UFUNCTION()
    void HandleDayChanged(int32 Year, int32 Day,const FString& Season, EWeatherType Weather);

    TSet<EWeatherType> WeatherTypes;

public:
#if WITH_EDITOR
    UFUNCTION(BlueprintCallable, CallInEditor)
    void TestSleep();
#endif

};
