
#include "Actors/Spawner/SpawnManager.h"

#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "NavFilters/NavigationQueryFilter.h"
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
            bAsyncComplete = true;
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

    GetWorld()->GetTimerManager().SetTimer(
    NavCheckHandle,
    this,
    &ThisClass::ReadyToSpawn,
    .5,
    true);
}

void ASpawnManager::ReadyToSpawn()
{
    bool bNavigationBeingBuilt = UNavigationSystemV1::GetNavigationSystem(this)->IsNavigationBeingBuilt(this);

    if (bNavigationBeingBuilt || !bAsyncComplete)
    {
        return;
    }

	GetWorld()->GetTimerManager().PauseTimer(NavCheckHandle);

    SpawnAssets(SpawnTypes[SpawnIndexCounter]);

    if (++SpawnIndexCounter >=SpawnTypes.Num())
    {
        SpawnIndexCounter = -1;
        GetWorld()->GetTimerManager().ClearTimer(NavCheckHandle);
    }
    GetWorld()->GetTimerManager().UnPauseTimer(NavCheckHandle);
}

void ASpawnManager::SpawnAssets(const FSpawnData& InSpawnData)
{
    checkf(NavigationData,TEXT("NavigationData is NULL"));
    checkf(NavMeshBoundsVolume,TEXT("NavMeshBoundsVolume is NULL"));

    int32 SpawnCount = CalculateSpawnCountByFarmSizePercentage(InSpawnData.SpawnRatePerFarmSize);

    FVector NavOrigin, Extent;
    NavMeshBoundsVolume->GetActorBounds(false, NavOrigin, Extent);
    float NavRadius = FMath::Max(Extent.X, Extent.Y) * 1.25;

    // for (int Index = 0; Index < SpawnCount ;++Index)
    // {
    //     if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
    //     {
    //         FNavLocation RandomLocation;
    //
    //
    //          if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius,RandomLocation, NavigationData))
    //          {
    //              checkf(InSpawnData.ClassRef.IsValid(),TEXT("SpawnParams.ClassRef is NULL"));
    //              FActorSpawnParameters SpawnParam;
    //              SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //
    //              GetWorld()->SpawnActor<AActor>(InSpawnData.ClassRef.Get(), RandomLocation.Location, FRotator::ZeroRotator, SpawnParam);
    //              NavSystem->Build();
    //          }
    //     }
    // }

    // 헤더에 FTimerHandle 선언 필요
    FTimerHandle SpawnTimerHandle;
    int32 SpawnIndex = 0; // for문 인덱스 대신 사용

    // 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        SpawnTimerHandle,
        [this, NavOrigin, NavRadius, InSpawnData, SpawnIndex, SpawnCount, SpawnTimerHandle]() mutable
        {
            if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                FNavLocation RandomLocation;

                if (NavSystem->GetRandomPointInNavigableRadius(NavOrigin, NavRadius, RandomLocation, NavigationData))
                {
                    checkf(InSpawnData.ClassRef.IsValid(), TEXT("SpawnParams.ClassRef is NULL"));

                    FActorSpawnParameters SpawnParam;
                    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                    GetWorld()->SpawnActor<AActor>(
                        InSpawnData.ClassRef.Get(),
                        RandomLocation.Location,
                        FRotator::ZeroRotator,
                        SpawnParam
                    );

                    // NavMesh 갱신
                    NavSystem->Build();
                }
            }

            // 다음 스폰 인덱스 증가
            SpawnIndex++;
            if (SpawnIndex >= SpawnCount)
            {
                // 타이머 종료
                GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
            }

        },
        1.f,   // 0.5초 간격
        true    // 반복
    );
}



