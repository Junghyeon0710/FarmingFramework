// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FootStepComponent.h"


UFootStepComponent::UFootStepComponent()
{

    PrimaryComponentTick.bCanEverTick = true;

}

void UFootStepComponent::FootStep(ACharacter* InCharacter, const FName& InSocketName, float InVolumeScale, float InVfxScale)
{

}

void UFootStepComponent::BeginPlay()
{
    Super::BeginPlay();


}


