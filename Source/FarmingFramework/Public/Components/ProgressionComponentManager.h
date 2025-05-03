// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProgressionComponentManager.generated.h"

USTRUCT()
struct FProgressionState
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FGameplayTag> InitTags;
};

/**
 * 만약에 2개가 동시에 등록하면 어떻게 되는지 체킈
 */
UCLASS()
class FARMINGFRAMEWORK_API UProgressionComponentManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UProgressionComponentManager();
	
	/** Utility to get this manager from an actor, will return null if actor is null or not in a world */
	static UProgressionComponentManager* GetForActor(const AActor* Actor, bool bOnlyGameWorlds = true);
	

private:
	
};
