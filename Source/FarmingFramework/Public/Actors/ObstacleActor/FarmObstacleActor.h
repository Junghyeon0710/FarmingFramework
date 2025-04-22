// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/TileItemBase.h"
#include "GameFramework/Actor.h"
#include "FarmObstacleActor.generated.h"

class USphereComponent;

UCLASS()
class FARMINGFRAMEWORK_API AFarmObstacleActor : public ATileItemBase
{
	GENERATED_BODY()

public:

	AFarmObstacleActor();

protected:

	UFUNCTION()
	virtual void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	
};
