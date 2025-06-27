// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "ItemFragment_WaterGiver.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_WaterGiver : public UFarmItemFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;
	virtual FGameplayTag GetFunctionTag() const override;
private:

	UPROPERTY(EditAnywhere)
	float TileDistance;
};
