// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "Montage/MontageFragment.h"
#include "ItemFragment_GroundLeveling.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_GroundLeveling : public UMontageFragment
{
	GENERATED_BODY()

public:
    virtual bool OnInteract() override;
	virtual FGameplayTag GetFunctionTag() const override;
    virtual AActor* GetInteractableActor() override;

    UPROPERTY(EditAnywhere, Category = CollisionChannel)
    TEnumAsByte<ECollisionChannel> GroundChannel = ECollisionChannel::ECC_Visibility;

    UPROPERTY(EditAnywhere, Category = Ground)
    TSubclassOf<AActor> GroundActorClass;

    UPROPERTY(EditAnywhere, Category = Ground)
    float GridRange = 100.0f;

    UPROPERTY(EditAnywhere, Category = Ground)
    FGameplayTag IgnoreDestroyTag;

    virtual void OnMontageEnd() override;

public:
    void SpawnGroundActor();
public:
    void SetIgnoreDestroyTag(const FGameplayTag& InTag) {IgnoreDestroyTag = InTag;}
    void SetGirdRange(float InRange) {GridRange = InRange;}
    void SetGroundActorClass(TSubclassOf<AActor> InClass) {GroundActorClass = InClass;}
    void SetGroundChannel(ECollisionChannel InChannel) {GroundChannel = InChannel;}

private:
    TWeakObjectPtr<AActor> CachedDetectedActor = nullptr;
};
