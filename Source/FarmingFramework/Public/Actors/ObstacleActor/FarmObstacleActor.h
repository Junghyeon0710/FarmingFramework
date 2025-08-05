// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\FarmTaggedActor.h"
#include "GameFramework/Actor.h"
#include "FarmObstacleActor.generated.h"

class UFarm_HighlightableStaticMesh;
class USphereComponent;

UCLASS()
class FARMINGFRAMEWORK_API AFarmObstacleActor : public AFarmTaggedActor
{
	GENERATED_BODY()

public:

	AFarmObstacleActor();

protected:

	// UFUNCTION()
	// virtual void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	// UFUNCTION()
	// virtual void OnEndOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFarm_HighlightableStaticMesh> HighlightableMesh;

	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USphereComponent> Collision;


};
