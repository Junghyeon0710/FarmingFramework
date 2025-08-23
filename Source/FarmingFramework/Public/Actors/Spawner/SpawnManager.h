// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class UFarmSpawnDataAsset;
class ANavMeshBoundsVolume;


class ANavigationData;

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<AActor> ClassRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnRatePerFarmSize = 0.1f;

    int32 TotalSpawnCount = 0;
    int32 CurrentSpawnCount = 0;
    bool bSpawnCompleted = false;

    void IncrementSpawnCount()
    {
        CurrentSpawnCount++;
        bSpawnCompleted = CurrentSpawnCount == TotalSpawnCount;
    }
};

UCLASS(Abstract, MinimalAPI)
class ASpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ASpawnManager();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavigationData> NavigationData;

    UPROPERTY(EditAnywhere, Category = "Navigation")
    TObjectPtr<ANavMeshBoundsVolume> NavMeshBoundsVolume;

    FTimerHandle NavCheckHandle;

    /** 네비게이션 볼륨 실제 사이즈를 구합니다. */
    FVector GetNavVolumeSize() const;

    /** 네비게이션 매쉬와 에셋이 준비 됐는지 */
    void WaitForNavMeshAndAssets();

    /** 네비게이션 체크 타이머를 시작합니다. */
    void StartNavCheckTimer();

private:
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<FSpawnData> SpawnTypes;

    int32 ClassRefIndex;
    bool bAsyncCompleted;
    int32 SpawnIndexCounter;
    bool bSpawnCompleted;

    /**
    * 현재 네비게이션 영역 크기에 맞춰 스폰 개수를 계산합니다.
    * 예) 100칸 중 10% 비율이면 → 10개 스폰
    *
    * @param SpawnRatePercent 스폰 비율(퍼센트)
    * @return 계산된 스폰 개수
    */
    int32 CalculateSpawnCountByFarmSizePercentage(float SpawnRatePercent) const;

     /** 스폰에 필요한 모든 클래스를 비동기로 불러옵니다. */
    void AsyncLoadClasses();

    /** 스폰에 필요한 단일 클래스를 비동기로 불러옵니다. */
    void AsyncLoadClass();

    /** 스폰 준비가 완료되면, 실제 스폰을 시작합니다. */
    void ReadyToSpawn();

    /**
     * 지정된 데이터를 기반으로 에셋을 스폰합니다.
     * @param InSpawnData 스폰에 필요한 데이터
     */
    void SpawnAssets(FSpawnData& InSpawnData);

    /** “스폰 완료 상태를 재검사합니다. */
    void RecheckSpawnCompletion();

    FTimerHandle SpawnTimerHandle;
private:


};
