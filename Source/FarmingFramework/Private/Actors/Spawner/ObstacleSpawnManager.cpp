// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner/ObstacleSpawnManager.h"

#include "DynamicWeatherSubsystem.h"
#include "Actors/ObstacleActor/FarmObstacleActor.h"

AObstacleSpawnManager::AObstacleSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawnManager::BeginPlay()
{
    Super::BeginPlay();

    if (UDynamicWeatherSubsystem* DynamicSubsystem = UDynamicWeatherSubsystem::Get(GetWorld()))
    {
        DynamicSubsystem->OnDayChanged.AddDynamic(this, ThisClass::HandleDayChanged);
        WeatherTypes.Add(DynamicSubsystem->GetWeatherType());
    }

}

bool AObstacleSpawnManager::CanSpawn(const FSpawnData& InSpawnData) const
{
    if (InSpawnData.ClassRef == nullptr)
    {
        return false;
    }

    AFarmObstacleActor* FarmObstacleActor = Cast<AFarmObstacleActor>(InSpawnData.ClassRef->ClassDefaultObject);
    if (!FarmObstacleActor)
    {
        return false;
    }

    if (FarmObstacleActor->GetWeatherType() == EWeatherType::None)
    {
        return true;
    }

    for (EWeatherType Weather : WeatherTypes)
    {
        if (Weather == FarmObstacleActor->GetWeatherType())
        {
            return true;
        }
    }

    return false;
}

void AObstacleSpawnManager::HandleDayChanged(int32 Year, int32 Day, const FString& Season, EWeatherType Weather)
{
    WeatherTypes.Add(Weather);
}


