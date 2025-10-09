// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FarmPlotActor.h"

#include "NavigationSystem.h"
#include "Builders/CubeBuilder.h"
#include "Components/BoxComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"


AFarmPlotActor::AFarmPlotActor()
{
    PrimaryActorTick.bCanEverTick = false;

    NavBoxBoundVolume = CreateDefaultSubobject<UBoxComponent>("NavBoxBoundVolume");
    NavBoxBoundVolume->SetBoxExtent(FVector(500,500,32));

}

void AFarmPlotActor::BeginPlay()
{
    Super::BeginPlay();

}

void AFarmPlotActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (!NavMeshBoundsVolume || !NavMeshBoundsVolume.ToSoftObjectPath().IsValid())
    {
        return;
    }

    if (!NavMeshBoundsVolume.IsValid())
    {
        ANavMeshBoundsVolume* LoadedVolume = NavMeshBoundsVolume.LoadSynchronous();
        if (!LoadedVolume)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load NavMeshBoundsVolume in %s"), *GetName());
            return;
        }
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded NavMeshBoundsVolume: %s"), *LoadedVolume->GetName());
    }

    NavMeshBoundsVolume->SetActorTransform(GetActorTransform());

    if (!NavBoxBoundVolume->GetScaledBoxExtent().Equals(NavMeshBoundsVolume->GetBounds().BoxExtent,0.001f))
    {
        UpdateNavMeshBoundsVolumeFromBox();
    }
}

void AFarmPlotActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

#if WITH_EDITOR
    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        UpdateNavMeshBoundsVolume();
    }
#endif
}

void AFarmPlotActor::PostEditMove(bool bFinished)
{
    Super::PostEditMove(bFinished);

   if (!bFinished)
   {
       return;
   }

    UpdateNavMeshBoundsVolume();
}

void AFarmPlotActor::UpdateNavMeshBoundsVolume()
{
#if WITH_EDITOR
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (GIsEditor && NavSys)
    {
        NavSys->OnNavigationBoundsUpdated(NavMeshBoundsVolume.Get());
    }
#endif
}

void AFarmPlotActor::UpdateNavMeshBoundsVolumeFromBox()
{
#if WITH_EDITOR
    if (UCubeBuilder* Cube = Cast<UCubeBuilder>(NavMeshBoundsVolume->BrushBuilder))
    {
        Cube->X = NavBoxBoundVolume->GetScaledBoxExtent().X * 2;
        Cube->Y = NavBoxBoundVolume->GetScaledBoxExtent().Y * 2;
        Cube->Z = NavBoxBoundVolume->GetScaledBoxExtent().Z * 2;

        if (ABrush* Brush = Cast<ABrush>(Cube->GetOuter()))
        {
            FPropertyChangedEvent PropertyChangedEvent(nullptr);
            Cube->PostEditChangeProperty(PropertyChangedEvent);
            NavMeshBoundsVolume->PostEditChangeProperty(PropertyChangedEvent);
            UpdateNavMeshBoundsVolume();
        }
    }
#endif

}



