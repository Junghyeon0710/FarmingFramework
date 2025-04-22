// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_RemoveBase.h"
#include "ItemFragment_RockRemove.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_RockRemove : public UItemFragment_RemoveBase
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetFunctionTag() const override;
};
