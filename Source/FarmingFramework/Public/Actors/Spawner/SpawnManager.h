// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class ANavMeshBoundsVolume;

USTRUCT(BlueprintType)
struct FSpawnData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<AActor> ClassRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnRatePerFarmSize = 0.1f; // 농작지 크기 비례 비율
};

class ANavigationData;

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

    /* 네비게이션 볼륨 실제 사이즈를 구합니다. */
    FVector GetNavVolumeSize() const;

private:
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<FSpawnData> SpawnData;

    int32 ClassRefIndex;
    bool bAsyncComplete;

    /**
     * 네비게이션 사이즈를 비례해서 스폰해야 할 카운터를 구합니다. EX)100칸 사용 중 -> 10개 생성
     * @param SpawnRatePercent 스폰 퍼센트 비율
     * @return 스폰 갯수
     */
    int32 CalculateSpawnCountByFarmSizePercentage(float SpawnRatePercent) const;

    /** 비동기로 스폰 클래스들을 로드 합니다.*/
	void AsyncLoadClasses();

    /** 비동기로 스폰 클래스를 로드 합니다 */
    void AsyncLoadClass();

};
