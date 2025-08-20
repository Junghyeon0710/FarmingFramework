// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\FarmTaggedActor.h"
#include "GameFramework/Actor.h"
#include "FarmObstacleActor.generated.h"

class UBoxComponent;
class UFarm_HighlightableStaticMesh;
class USphereComponent;

UCLASS()
class FARMINGFRAMEWORK_API AFarmObstacleActor : public AFarmTaggedActor
{
	GENERATED_BODY()

public:

	AFarmObstacleActor();

protected:

    virtual void OnConstruction(const FTransform& Transform) override;

    // UFUNCTION()
	// virtual void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	// UFUNCTION()
	// virtual void OnEndOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

public:
    virtual void OnInteract(AActor* Interactor);
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFarm_HighlightableStaticMesh> HighlightableMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collision;

    UPROPERTY(EditAnywhere, Category = "RandomeMesh")
    bool bUseRandomMesh = false;

    UPROPERTY(EditAnywhere, Category = "RandomeMesh")
    TArray<TSoftObjectPtr<UStaticMesh>> RandomMeshList;

protected:

    UPROPERTY(EditAnywhere, Category = "Remove")
    TArray<TSoftObjectPtr<UStaticMesh>> RemoveMeshList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Remove")
    int32 RequiredInteractions = 1;

    int32 CurrentInteractionCount = 0;

private:
    void LoadAndSetMeshAsync();
    void UpdateRequiredInteractionsFromHighlightMesh();


};
