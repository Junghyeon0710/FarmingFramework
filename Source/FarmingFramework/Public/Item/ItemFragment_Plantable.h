// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ignore/ItemFragment_LineTraceFilterByIgnoreTag.h"
#include "Montage/MontageFragment.h"
#include "ItemFragment_Plantable.generated.h"

UENUM(BlueprintType)
enum class EActorPivotPosition : uint8
{
    Center UMETA(DisplayName = "Center"),
    Bottom UMETA(DisplayName = "Bottom")
};

UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_Plantable : public UItemFragment_LineTraceFilterByIgnoreTag
{
	GENERATED_BODY()

public:
	//virtual void OnInteract() override;
    virtual void OnInteractSuccess(AActor*& DetectedActor) override;

    EActorPivotPosition GetActorPivotPosition(const AActor* Actor);
    EActorPivotPosition GetStaticMeshPivotPosition(const UStaticMeshComponent* MeshComp);
    EActorPivotPosition GetSkeletalMeshPivotPosition(const USkeletalMeshComponent* MeshComp);
private:

	// UFUNCTION()
	// void StartProgression(AActor* Actor);

	UPROPERTY(EditAnywhere, Category="FarmingProgression")
	TSubclassOf<AActor> SeedActorClass;

	//TWeakObjectPtr<AActor> SeedActor;
};
