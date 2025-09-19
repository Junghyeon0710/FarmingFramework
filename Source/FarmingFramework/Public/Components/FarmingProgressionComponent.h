// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressionComponent.h"
#include "Interface/ProgressionStateInterface.h"
#include "FarmingProgressionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFarmingProgressionComponent : public UProgressionComponent , public IProgressionStateInterface
{
	GENERATED_BODY()

public:
	UFarmingProgressionComponent();

	/** IProgressionStateInterface*/
	virtual bool CanChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleFailedInitStateChange(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	/** ~IProgressionStateInterface */

private:

    UPROPERTY(EditAnywhere, Category = Config)
    TSoftObjectPtr<UStaticMesh> FailedMesh;

public:
    FORCEINLINE TSoftObjectPtr<UStaticMesh> GetFailedMesh() const { return FailedMesh; }
    FORCEINLINE void SetFailedMesh(TSoftObjectPtr<UStaticMesh> InFailedMesh) { FailedMesh = InFailedMesh; }
};
