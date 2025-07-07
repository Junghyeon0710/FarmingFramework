// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Montage/MontageFragment.h"
#include "ItemFragment_WaterGiver.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_WaterGiver : public UMontageFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;
	virtual FGameplayTag GetFunctionTag() const override;

	bool HasIgnoreTag(AActor* InActor);
private:

	UPROPERTY(EditAnywhere)
	float TileDistance;

	UPROPERTY(EditAnywhere)
	FGameplayTag IgnoreTag;

	TArray<TWeakObjectPtr<AActor>> IgnoreActors;
};
