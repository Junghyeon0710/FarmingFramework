// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActors/Tiles/Farm_TileActor.h"


AFarm_TileActor::AFarm_TileActor(const FObjectInitializer& ObjectInitializer)
{
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
}

void AFarm_TileActor::Interact()
{
	UMaterialInstanceDynamic* DynMat = TileMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMat)
	{
		DynMat->SetVectorParameterValue("SoilTint", FLinearColor(0.015, 0.006f, 0)); // 진한 갈색
		StaticGameplayTags.Reset();
	}
}
