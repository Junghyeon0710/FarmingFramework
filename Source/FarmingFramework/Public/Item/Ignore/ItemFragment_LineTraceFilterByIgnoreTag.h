// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "Item/Montage/MontageFragment.h"
#include "ItemFragment_LineTraceFilterByIgnoreTag.generated.h"

/**
 * 라인트레이서 진행시 히트액터에 무시 태그가 있으면 무시하고 다시 TileDistance 범위에 있는 액터를 체크
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_LineTraceFilterByIgnoreTag : public UMontageFragment
{
    GENERATED_BODY()

public:
	virtual void OnInteract() override;

    virtual void OnInteractSuccess(AActor* DetectedActor) {};
protected:
    bool HasIgnoreTag(AActor* InActor);

    UPROPERTY(EditAnywhere, meta = (Categories = "Ignore"))
    FGameplayTag IgnoreTag;

    TArray<TWeakObjectPtr<AActor>> IgnoreActors;
};
