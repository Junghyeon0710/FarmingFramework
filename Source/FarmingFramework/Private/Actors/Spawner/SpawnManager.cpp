
#include "Actors/Spawner/SpawnManager.h"

#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavMesh/RecastNavMesh.h"


ASpawnManager::ASpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;

}

void ASpawnManager::BeginPlay()
{
    Super::BeginPlay();

    AsyncLoadClasses();
}

FVector ASpawnManager::GetNavVolumeSize() const
{
    checkf(NavMeshBoundsVolume,TEXT("NavMeshBoundsVolume is NULL"));

    FVector Origin = FVector(0, 0, 0);
    FVector Extent = FVector(0, 0, 0);

    NavMeshBoundsVolume->GetActorBounds(false, Origin, Extent);

    return Extent * 2;
}

int32 ASpawnManager::CalculateSpawnCountByFarmSizePercentage(float SpawnRatePercent) const
{
    FVector TileSize = GetNavVolumeSize() / 100;

    return TileSize.X * TileSize.Y * SpawnRatePercent;
}

void ASpawnManager::AsyncLoadClasses()
{
    ClassRefIndex = 0;
    bAsyncComplete = false;
}

void ASpawnManager::AsyncLoadClass()
{
    FSoftObjectPath SoftObjectPath(SpawnData[ClassRefIndex].ClassRef.ToSoftObjectPath());
    UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this, SoftObjectPath]()
    {
        ClassRefIndex++;
        if (ClassRefIndex > SpawnData.Num()-1)
        {
            bAsyncComplete = true;
        }
        else
        {
            AsyncLoadClass();
        }
   }));
}



