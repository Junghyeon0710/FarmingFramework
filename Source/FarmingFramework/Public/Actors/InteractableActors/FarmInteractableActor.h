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
	virtual void Interact() {};
	
};
