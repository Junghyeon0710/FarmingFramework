// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStep.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UAnimNotify_FootStep : public UAnimNotify
{
    GENERATED_BODY()

private:

    /** UAnimNotify */
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) final;
    /** ~End UAnimNotify */

private:

    UPROPERTY(EditDefaultsOnly, Category = "FootStep")
    float VolumeScale = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "FootStep")
    FName SockentName;

};
