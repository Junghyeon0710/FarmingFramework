// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_GroundFlatten.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_GroundFlatten : public UFarmItemFragment
{
	GENERATED_BODY()

public:
    virtual void OnInteract() override;
	virtual FGameplayTag GetFunctionTag() const override;
    virtual AActor* GetInteractableActor() override;

    UPROPERTY(EditAnywhere, Category = CollisionChannel)
    TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility;

    UPROPERTY(EditAnywhere, Category = Ground)
    TSubclassOf<AActor> GroundActorClass;

    UPROPERTY(EditAnywhere, Category = Ground)
    float GridRange = 100.0f;


public:
    void SpawnGroundActor();
};
