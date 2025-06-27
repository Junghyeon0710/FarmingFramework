// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActors/FarmInteractableActor.h"
#include "Farm_TileActor.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API AFarm_TileActor : public AFarmInteractableActor
{
	GENERATED_BODY()

public:
	AFarm_TileActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Interact() override;


private:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TileMesh;
};
