// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProgressionManagerComponent.generated.h"

class UStaticMesh;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UProgressionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProgressionManagerComponent();

	virtual float Interact();
	virtual void SetProgressionState(float Progression);
	void FarmingProgress(float& OutDelay, int32& Stage);
	void SwitchStage();
	void SetReady();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = List)
	TArray<TObjectPtr<UStaticMesh>> MeshList;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float ProgressionState = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CollectionTime = 3.f;
};
