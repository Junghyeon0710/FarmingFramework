// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProgressionComponentManager.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UProgressionComponentManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UProgressionComponentManager();
	
	/** Utility to get this manager from an actor, will return null if actor is null or not in a world */
	static UProgressionComponentManager* GetForActor(const AActor* Actor, bool bOnlyGameWorlds = true);

	void RegisterInitTags(const TArray<FGameplayTag>& InInitTags);
	void UnRegisterInitTags();

	FGameplayTag GetCurrentState();
	FGameplayTag GetDesiredState();
	void NextInitState();

private:
	
	UPROPERTY()
	TArray<FGameplayTag> InitTags;

	int32 InitState = 0;
};
