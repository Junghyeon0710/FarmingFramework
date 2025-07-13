// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Montage/MontageFragment.h"
#include "ItemFragment_Plantable.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_Plantable : public UMontageFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;

private:

	UFUNCTION()
	void StartProgression(AActor* Actor);

	UPROPERTY(EditAnywhere, Category="FarmingProgression")
	TSubclassOf<AActor> SeedActorClass;

	//TWeakObjectPtr<AActor> SeedActor;
};
