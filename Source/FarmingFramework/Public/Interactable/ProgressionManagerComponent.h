// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ProgressionManagerComponent.generated.h"

class UStaticMesh;

USTRUCT(BlueprintType)
struct FMeshList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	float TransitionTime;

	UPROPERTY(EditAnywhere)
	FGameplayTag ProgressionState;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UProgressionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProgressionManagerComponent();

	UFUNCTION(BlueprintCallable)
	virtual float Interact();
	virtual void SetProgressionState(float Progression);
	void FarmingProgress(float& OutDelay, int32& Stage);
	void SwitchStage();
	void SetReady();

	FTimerHandle ProgressTimer;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = List)
	TArray<FMeshList> MeshList;

	int32 ProgressState =0;

	UPROPERTY()
	UStaticMeshComponent* NewStaticMeshComp = nullptr;
};
