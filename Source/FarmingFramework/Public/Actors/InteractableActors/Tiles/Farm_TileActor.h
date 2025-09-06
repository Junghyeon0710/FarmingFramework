// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeasonWeatherData.h"
#include "Actors/InteractableActors/FarmInteractableActor.h"
#include "Components/Building/BuildTargetInterface.h"
#include "Farm_TileActor.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API AFarm_TileActor : public AFarmInteractableActor, public IBuildTargetInterface
{
	GENERATED_BODY()

public:
	AFarm_TileActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnInteract() override;
	void SetSeed(AActor* InSeed) { Seed = InSeed; };
    AActor* GetSeed() const { return Seed; }
    bool TryCropHarvest(float LifeSpan);
    virtual void OnCropHarvest() {};

	/** IBuildTargetInterface */
	virtual void OnBuildingPlaced_Implementation(AActor* PlacedActor) override;
	virtual bool CanBeBuiltOn_Implementation() override;
	/** ~IBuildTargetInterface */

protected:
	virtual void BeginPlay() override;


protected:
	/** Dynamic Weather*/
	UFUNCTION()
	void OnDayChange( int32 Year, int32 Day,const FString& Season,  EWeatherType Weather);

protected:
	void ApplyWetSoilVisual();

private:
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynMaterial;

    UPROPERTY(EditAnywhere, Category=DynamicMaterial)
    FName MultiplyColor_Wet = "Multiply Color";

    UPROPERTY(EditAnywhere, Category=DynamicMaterial)
    FLinearColor MultiplyColor_LinearColor{0.417885f, 0.417885f, 0.417885, 1};

	UPROPERTY()
	TObjectPtr<AActor> Seed = nullptr;

	void InitializeDynamicMaterial();

private:
    UPROPERTY(EditAnywhere, Category = "Tile")
    TObjectPtr<UStaticMesh> TileMesh;

    UPROPERTY(EditAnywhere, Category = "Tile")
    TObjectPtr<UStaticMesh> TileMesh_L;

    UPROPERTY(EditAnywhere, Category = "Tile")
    TObjectPtr<UStaticMesh> TileMesh_M;

    UPROPERTY(EditAnywhere, Category = "Tile")
    TObjectPtr<UStaticMesh> TileMesh_R;


    TWeakObjectPtr<AFarm_TileActor> LeftTile;
    TWeakObjectPtr<AFarm_TileActor> RightTile;
};
