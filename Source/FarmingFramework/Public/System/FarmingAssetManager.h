// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FarmingAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UFarmingAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UFarmingAssetManager& Get();
};
