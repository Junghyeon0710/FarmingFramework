// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootStepComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFootStepComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootStepComponent();

    /* 발 닿을시 기능을 수행합니다. */
    void FootStep(ACharacter* InCharacter, const FName& InSocketName, float InVolumeScale = 1.f, float InVfxScale = 1.f);

protected:
    virtual void BeginPlay() override;


};
