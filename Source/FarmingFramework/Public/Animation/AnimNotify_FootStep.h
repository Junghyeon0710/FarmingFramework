// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStep.generated.h"

/**
 *
 */
UCLASS(meta=(DisplayName="Foot Step"))
class FARMINGFRAMEWORK_API UAnimNotify_FootStep : public UAnimNotify
{
    GENERATED_BODY()

private:

    /** UAnimNotify */
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override final;
    /** ~End UAnimNotify */

private:

    /** 발소리 봄륨 배율 */
    UPROPERTY(EditAnywhere, Category="AnimNotify")
    float VolumeScale = 1.f;

    /** 발소리 Vfx 배율 */
    UPROPERTY(EditAnywhere, Category="AnimNotify")
    float VfxScale = 1.f;

    /** 발소리가 적용 될 소켓 이름 */
    UPROPERTY(EditAnywhere, Category="AnimNotify")
    FName SocketName;

};
