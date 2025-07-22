// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "Item/Montage/MontageFragment.h"
#include "ItemFragment_RemoveBase.generated.h"

/**
 *
 */
UCLASS(Abstract)
class FARMINGFRAMEWORK_API UItemFragment_RemoveBase : public UMontageFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;
	virtual void OnInteractWithTag(FGameplayTag InFunctionTag) override;
	virtual void OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor) override;
};
