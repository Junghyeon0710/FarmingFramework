// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CropData.generated.h"

/**
 *
 */
USTRUCT()
struct FCropData : public FTableRowBase
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, Category = "Crop Data")
    FString Name;

    /** 작물 성장 그룹 ID */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 CropGrowthID;

    /** 수확에 필요한 재배 일수 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 GrowingDays;

    /** 작물 재배 시 일반 획득 보상 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 CropRewardIDNormal;

    /** 작물 재배 시 대성공 획득 보상 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 CropRewardIDLucky;

    /** 작물 건강에 피해주지 않는 온도 최소값 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 TemperatureMin;

    /** 작물 건강에 피해주지 않는 온도 최대값 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 TemperatureMax;

    /** 작물 건강 시작 최소값 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 InitCropHealthMin;

    /** 작물 건강 시작 최대값 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 InitCropHealthMax;

    /** 대성공을 위해 필요한 작물 건강 값 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 NeedCropHealthforLucky;

    /** 대성공 확률 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    int32 LuckyRewardRate;

    /** 병충해 악영향 유무 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    bool PestsRisk;

    /** 야생동물 악영향 유무 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    bool AnimalRisk;

    /** 야간 농사 미진행 시 악영향 유무 */
    UPROPERTY(EditAnywhere, Category = "Crop Data")
    bool NightRisk;
};
