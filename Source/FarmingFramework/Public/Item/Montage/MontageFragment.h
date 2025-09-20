// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "MontageFragment.generated.h"

/**
 *
 */
class UAnimMontage;

UCLASS()
class FARMINGFRAMEWORK_API UMontageFragment : public UFarmItemFragment
{
    GENERATED_BODY()
public:
    virtual void PlayMontage();
    void SetMontage(UAnimMontage* InMontage) { AnimMontage = InMontage; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Montage")
    TObjectPtr<UAnimMontage> AnimMontage;

    virtual void OnMontageEnd();

    float MontageLength;
};
