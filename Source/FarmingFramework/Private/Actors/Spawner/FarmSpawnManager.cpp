
#include "Actors/Spawner/FarmSpawnManager.h"

#include "NavigationSystem.h"
#include "Actors/ObstacleActor/FarmObstacleActor.h"
#include "Engine/AssetManager.h"
#include "NavMesh/NavMeshBoundsVolume.h"


AFarmSpawnManager::AFarmSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;

}

void AFarmSpawnManager::BeginPlay()
{
    Super::BeginPlay();

    AsyncLoadClasses();
}

FVector AFarmSpawnManager::GetNavVolumeSize() const
{
    checkf(NavMeshBoundsVolume,TEXT("NavMeshBoundsVolume is NULL"));

    FVector Origin = FVector(0, 0, 0);
    FVector Extent = FVector(0, 0, 0);

    NavMeshBoundsVolume->GetActorBounds(false, Origin, Extent);

    return Extent * 2;
}

int32 AFarmSpawnManager::CalculateSpawnCountByFarmSizePercentage(float SpawnRatePercent) const
{
    FVector TileSize = GetNavVolumeSize() / 100;

    return TileSize.X * TileSize.Y * SpawnRatePercent;
}

void AFarmSpawnManager::AsyncLoadClasses()
{
    ClassRefIndex = 0;
    bAsyncCompleted = false;

    AsyncLoadClass();
}

void AFarmSpawnManager::AsyncLoadClass()
{
    FSoftObjectPath SoftObjectPath(SpawnTypes[ClassRefIndex].ClassRef.ToSoftObjectPath());
    UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this, SoftObjectPath]()
    {
        ClassRefIndex++;
        if (ClassRefIndex > SpawnTypes.Num()-1)
        {
            bAsyncCompleted = true;
            WaitForNavMeshAndAssets();
        }
        else
        {
            AsyncLoadClass();
        }
   }));
}

void AFarmSpawnManager::WaitForNavMeshAndAssets()
{
    SpawnIndexCounter = 0;

    StartNavCheckTimer();
}

void AFarmSpawnManager::StartNavCheckTimer()
{
    bSpawnCompleted = true;
    if (!GetWorld()->GetTimerManager().IsTimerActive(NavCheckHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(NavCheckHandle, this, &ThisClass::ReadyToSpawn, .5, true);
    }
}

void AFarmSpawnManager::ReadyToSpawn()
{
    bool bNavigationBeingBuilt = UNavigationSystemV1::GetNavigationSystem(this)->IsNavigationBeingBuilt(this);

    if (bNavigationBeingBuilt || !bAsyncCompleted || !bSpawnCompleted)
    {
        return;
    }

    if (SpawnTypes.IsValidIndex(SpawnIndexCounter))
    {
        SpawnAssets(SpawnTypes[SpawnIndexCounter]);
    }

    if (++SpawnIndexCounter >SpawnTypes.Num())
    {
        SpawnIndexCounter = 0;
        GetWorld()->GetTimerManager().ClearTimer(NavCheckHandle);
        RecheckSpawnCompletion();
    }

}

void AFarmSpawnManager::SpawnAssets(FSpawnData& InSpawnData)
{
    if (bIsReSpawn && !CanSpawn(InSpawnData))
    {
        InSpawnData.bSpawnCompleted = true;
        return;
    }

    checkf(NavigationData,TEXT("NavigationData is NULL"));
    checkf(NavMeshBoundsVolume,TEXT("NavMeshBoundsVolume is NULL"));
    bSpawnCompleted = false;

    if (InSpawnData.bSpawnCompleted)
    {
        bSpawnCompleted = true;
        return;
    }

    int32 SpawnCount = CalculateSpawnCountByFarmSizePercentage(InSpawnData.SpawnRatePerFarmSize);
    InSpawnData.TotalSpawnCount = SpawnCount;

    FVector NavOrigin, Extent;
    NavMeshBoundsVolume->GetActorBounds(false, NavOrigin, Extent);
    float NavRadius = FMath::Max(Extent.X, Extent.Y) * 1.25;

    int32 SpawnIndex = InSpawnData.CurrentSpawnCount;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return;
    }

    while (SpawnIndex < InSpawnData.TotalSpawnCount)
    {
        FNavLocation RandomLocation;
        if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius, RandomLocation, NavigationData))
        {
            if (InSpawnData.ClassRef.IsValid())
            {
                FActorSpawnParameters SpawnParam;
                SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                if (AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(InSpawnData.ClassRef.Get(), RandomLocation.Location,FRotator::ZeroRotator, SpawnParam))
                {
                    FTimerHandle TimerHandle;
                    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SpawnedActor, &InSpawnData]()
                    {
                        TArray<AActor*> Overlaps;
                        SpawnedActor->GetOverlappingActors(Overlaps, AFarmObstacleActor::StaticClass());
                        if (Overlaps.Num() > 0)
                        {
                            SpawnedActor->Destroy();
                        }
                        else
                        {
                            InSpawnData.IncrementSpawnCount();
                        }
                    }, 0.05f, false);
                }
            }
            SpawnIndex++;
        }
        else
        {
            break;
        }
    }

    bSpawnCompleted = true;

}

void AFarmSpawnManager::RecheckSpawnCompletion()
{
    if (RespawnCount > 5)
    {
        RespawnCount = 0;
        OnSpawnCompleted();
        return;
    }

    for (FSpawnData& Data : SpawnTypes)
    {
        if (Data.bSpawnCompleted)
        {
            continue;
        }

        FVector NavOrigin, Extent;
        NavMeshBoundsVolume->GetActorBounds(false, NavOrigin, Extent);
        float NavRadius = FMath::Max(Extent.X, Extent.Y) * 1.25;
        if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            FNavLocation RandomLocation;
           // NavigationData->RebuildAll();
            if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius, RandomLocation, NavigationData))
            {
                StartNavCheckTimer();
                RespawnCount++;
                return;
            }
        }
    }
    OnSpawnCompleted();
}

void AFarmSpawnManager::ReSpawn()
{
    bIsReSpawn = true;
    WaitForNavMeshAndAssets();
}

void AFarmSpawnManager::OnSpawnCompleted()
{
    UE_LOG(LogTemp, Warning, TEXT("OnSpawnCompleted"));
    for (FSpawnData& SpawnData : SpawnTypes)
    {
        SpawnData.Reset();
    }
}



