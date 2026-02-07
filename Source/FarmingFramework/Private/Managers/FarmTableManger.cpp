// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/FarmTableManger.h"

#include "Data/CropData.h"

void UFarmTableManger::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    LoadDataTableToMap<FCropData>(TEXT("/Script/Engine.DataTable'/Game/Project/Farm/Data/DT_CropData.DT_CropData'"), CropDataMap);
    UE_LOG(LogTemp, Warning, TEXT("FarmTableManger Initialized"));
}
