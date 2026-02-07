// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FarmTableManger.generated.h"

struct FCropData;
/**
 *
 */
UCLASS()
class FARMINGFRAMEWORK_API UFarmTableManger : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    /** Begin USubSystem */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    /** ~End USubsystem */

public:

    TMap<int32, TSharedPtr<FCropData>> CropDataMap;




public:

template<typename RowStruct>
bool LoadDataTableToMap(const FString& DataTablePath, TMap<int32, TSharedPtr<RowStruct>>& OutDataMap)
    {
        static_assert(TIsDerivedFrom<RowStruct, FTableRowBase>::IsDerived,"RowStruct must derive from FTableRowBase");

        const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
        if (!DataTable)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load DataTable : %s"), *DataTablePath);
            return false;
        }

        TArray<RowStruct*> Rows;
        DataTable->GetAllRows(TEXT("LoadDataTableToMap"), Rows);

        const TArray<FName> RowNames = DataTable->GetRowNames();

        if (Rows.Num() != RowNames.Num())
        {
            UE_LOG(LogTemp, Warning, TEXT("Row count mismatch : %s"), *DataTablePath);
            return false;
        }

        OutDataMap.Empty(Rows.Num());

        for (int32 Index = 0; Index < Rows.Num(); ++Index)
        {
            const int32 ID = FCString::Atoi(*RowNames[Index].ToString());
            OutDataMap.Emplace(ID, MakeShared<RowStruct>(*Rows[Index]));
        }

        return true;
    }

};
