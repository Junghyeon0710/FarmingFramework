
#include "Actors/Spawner/SpawnManager.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "NavMesh/NavMeshBoundsVolume.h"


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
    bAsyncCompleted = false;

    AsyncLoadClass();
}

void ASpawnManager::AsyncLoadClass()
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

void ASpawnManager::WaitForNavMeshAndAssets()
{
    SpawnIndexCounter = 0;

    StartNavCheckTimer();
}

void ASpawnManager::StartNavCheckTimer()
{
    bSpawnCompleted = true;
    if (!GetWorld()->GetTimerManager().IsTimerActive(NavCheckHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(NavCheckHandle, this, &ThisClass::ReadyToSpawn, .5, true);
    }
}

void ASpawnManager::ReadyToSpawn()
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

void ASpawnManager::SpawnAssets(FSpawnData& InSpawnData)
{

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

    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,[this, NavOrigin, NavRadius, &InSpawnData, SpawnIndex, SpawnCount]() mutable
        {
            if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {

                FNavLocation RandomLocation;

                if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius, RandomLocation, NavigationData))
                {
                    checkf(InSpawnData.ClassRef.IsValid(), TEXT("SpawnParams.ClassRef is NULL"));

                    FActorSpawnParameters SpawnParam;
                    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

                   if (AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(InSpawnData.ClassRef.Get(), RandomLocation.Location, FRotator::ZeroRotator,SpawnParam))
                   {
                       InSpawnData.IncrementSpawnCount();
                   }
                }
            }

            if (++SpawnIndex >= SpawnCount)
            {
                GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
                bSpawnCompleted = true;
            }
        },.1f, true);

}

void ASpawnManager::RecheckSpawnCompletion()
{
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
            if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius, RandomLocation, NavigationData))
            {
                StartNavCheckTimer();
            }
        }
    }
}



