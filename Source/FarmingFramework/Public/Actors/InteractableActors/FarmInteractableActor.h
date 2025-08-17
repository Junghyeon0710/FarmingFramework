// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/FarmTaggedActor.h"
#include "FarmInteractableActor.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API AFarmInteractableActor : public AFarmTaggedActor
{
	GENERATED_BODY()

public:
	AFarmInteractableActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Interact() {};

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	float GridSize = 100;

	UPROPERTY(EditAnywhere)
	int32 ZScale{1};


};
