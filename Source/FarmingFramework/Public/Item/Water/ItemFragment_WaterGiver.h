// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Ignore/ItemFragment_LineTraceFilterByIgnoreTag.h"
#include "Item/Montage/MontageFragment.h"
#include "ItemFragment_WaterGiver.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_WaterGiver : public UItemFragment_LineTraceFilterByIgnoreTag
{
	GENERATED_BODY()

public:
    UItemFragment_WaterGiver();
	virtual void OnInteractSuccess(AActor* DetectedActor) override;

	virtual FGameplayTag GetFunctionTag() const override;

};
