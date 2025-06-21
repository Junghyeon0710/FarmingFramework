// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_Plantable.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_Plantable : public UFarmItemFragment
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
