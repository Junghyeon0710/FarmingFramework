// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

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


};
