// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_ItemCount.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_ItemCount : public UFarmItemFragment
{
	GENERATED_BODY()
	
public:
	int32 GetMaxCount() const {return MaxCount;}
	int32 GetCount() const {return Count;}
	void SetCount(int32 InCount) {Count = InCount;}

private:

	UPROPERTY(EditAnywhere, Category="Item")
	int32 MaxCount{1};

	UPROPERTY(EditAnywhere, Category="Item")
	int32 Count{1};
};
