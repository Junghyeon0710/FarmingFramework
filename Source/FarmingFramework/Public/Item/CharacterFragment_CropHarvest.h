// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ignore/ItemFragment_LineTraceFilterByIgnoreTag.h"
#include "CharacterFragment_CropHarvest.generated.h"

/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UCharacterFragment_CropHarvest : public UItemFragment_LineTraceFilterByIgnoreTag
{
	GENERATED_BODY()

public:
    virtual void OnInteractSuccess(AActor* DetectedActor);

};
