// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/ProgressionComponent.h"
#include "Interface/ProgressionStateInterface.h"
#include "FarmingProgressionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFarmingProgressionComponent : public UProgressionComponent , public IProgressionStateInterface
{
	GENERATED_BODY()

public:
	UFarmingProgressionComponent();

	/** IProgressionStateInterface*/

	virtual bool CanChangeInitState(UProgressionComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UProgressionComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleFailedInitStateChange(UProgressionComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	/** ~IProgressionStateInterface */

	UPROPERTY(EditAnywhere, Category = Test)
	bool bTest1;

	UPROPERTY(EditAnywhere, Category = Test)
	bool bTest2;

	UPROPERTY(EditAnywhere, Category = Test)
	bool bTest3;
};
