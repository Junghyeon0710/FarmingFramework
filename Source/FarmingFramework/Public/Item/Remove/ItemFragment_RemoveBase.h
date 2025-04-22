// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "ItemFragment_RemoveBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FARMINGFRAMEWORK_API UItemFragment_RemoveBase : public UFarmItemFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;
};
