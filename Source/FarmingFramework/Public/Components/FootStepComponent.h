// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootStepComponent.generated.h"

class USoundBase;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FFootStepInfo
{
    GENERATED_BODY()

    /* 풋스텝 타입 (ex 1:콘크리트 2:물) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FootStepInfo")
    int32 FootStepType;

    /* 발자국 소리 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FootStepInfo")
    TSoftObjectPtr<USoundBase> FootStepSound;

    /* 발자국 이펙트 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FootStepInfo")
    TSoftObjectPtr<UNiagaraSystem> FootStepVfx;

};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFootStepComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFootStepComponent();

    /* 발 닿을시 기능을 수행합니다. */
    void FootStep(ACharacter* InCharacter, const FName& InSocketName, float InVolumeScale = 1.f, float InVfxScale = 1.f);

    virtual void BeginPlay() override;

protected:

    /* 풋스텝 정보 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FootStepInfo")
    TMap<FName, FFootStepInfo> FootStepInfos;
};
