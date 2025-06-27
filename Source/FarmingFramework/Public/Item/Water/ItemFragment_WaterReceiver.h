// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FarmItemFragment.h"
#include "ItemFragment_WaterReceiver.generated.h"

/**
 * 물을 받는 기능
 * Ex) 우물
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_WaterReceiver : public UFarmItemFragment
{
	GENERATED_BODY()

public:
	virtual void OnInteract() override;
};
