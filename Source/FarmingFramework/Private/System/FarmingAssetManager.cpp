// Fill out your copyright notice in the Description page of Project Settings.


#include "System/FarmingAssetManager.h"

UFarmingAssetManager& UFarmingAssetManager::Get()
{
	check(GEngine);

	if (UFarmingAssetManager* Singleton = Cast<UFarmingAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to LyraAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UFarmingAssetManager>();
}


