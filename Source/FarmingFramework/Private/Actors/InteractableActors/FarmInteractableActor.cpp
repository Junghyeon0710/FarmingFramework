// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActors/FarmInteractableActor.h"


#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFarmInteractableActor::AFarmInteractableActor(const FObjectInitializer& ObjectInitializer)
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
}

void AFarmInteractableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

    if (!bUseRandomMesh || MeshList.IsEmpty()) return;

    Mesh->SetStaticMesh(MeshList[FMath::RandRange(0,MeshList.Num()-1)].LoadSynchronous());

	FVector LocalMin, LocalMax;
	Mesh->GetLocalBounds(LocalMin, LocalMax);

	FVector LocalSize = (LocalMax - LocalMin) * Mesh->GetComponentScale();

	FVector NormalizedMax = LocalSize / GridSize;

	int32 SnappedX = FMath::RoundToInt32(NormalizedMax.X) * GridSize;
	int32 SnappedY = FMath::RoundToInt32(NormalizedMax.Y) * GridSize;
	int32 SnappedZ = FMath::RoundToInt32(NormalizedMax.Z) * GridSize;

	int32 MaxXY = FMath::Max(SnappedX, SnappedY);

	FVector FinalExtent = FVector(
		FMath::Max(GridSize, MaxXY),
		FMath::Max(GridSize, MaxXY),
		FMath::Max(GridSize, SnappedZ)
	);

	FinalExtent.Z *= ZScale;
	Box->SetBoxExtent(FinalExtent / 2);
	Box->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1, 0, 0)));
}
