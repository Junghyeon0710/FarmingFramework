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
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Destroyed() override;

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
    /**
    * 지정된 방향으로 인접한 타일을 확인합니다.
    * @param Direction 탐색할 방향 벡터 (예: 좌/우 방향)
    * @return 해당 방향에 존재하는 AFarm_TileActor 포인터, 없으면 nullptr
    */
    AFarm_TileActor* CheckAdjacentTile(const FVector& Direction) const;

    bool CheckLeftTile();
    bool CheckRightTile();

    /** 둔덕을 연결합니다. */
    void ConnectRidgeWithNeighbors();

    /** 다시 인접 타일의 둔덕 상태를 갱신합니다.*/
    void RefreshAdjacentRidges();

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

    UPROPERTY(EditAnywhere, Category = "Tile")
    float TraceLength = 50.0f;
};
