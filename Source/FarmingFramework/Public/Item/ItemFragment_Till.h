// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_Till.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_Till : public UFarmItemFragment
{
	GENERATED_BODY()

	virtual void OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor) override;
	virtual FGameplayTag GetFunctionTag() const override;
};
