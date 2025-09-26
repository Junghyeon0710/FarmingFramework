// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeasonWeatherData.h"
#include "..\FarmTaggedActor.h"
#include "GameFramework/Actor.h"
#include "Interface/FarmInteractableInterface.h"
#include "Interface/FarmSpawnMangerInterface.h"
#include "FarmObstacleActor.generated.h"

class UBoxComponent;
class UFarm_HighlightableStaticMesh;
class USphereComponent;

UCLASS()
class FARMINGFRAMEWORK_API AFarmObstacleActor : public AFarmTaggedActor, public IFarmInteractableInterface, public IFarmSpawnMangerInterface
{
	GENERATED_BODY()

public:
	AFarmObstacleActor();

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

    // UFUNCTION()
	// virtual void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	// UFUNCTION()
	// virtual void OnEndOverlap( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

public:
    /** IFarmInteractableInterface */
    virtual void Interact(AActor* Interactor) override;
    /** ~IFarmInteractableInterface */

    /** IFarmSpawnMangerInterface */
    virtual void FinishSpawn() override;
    /** ~IFarmSpawnMangerInterface */

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

    UPROPERTY(EditAnywhere, Category = "Weather")
    EWeatherType WeatherType = EWeatherType::None;

public:
    FORCEINLINE EWeatherType GetWeatherType() const { return WeatherType; }
};
