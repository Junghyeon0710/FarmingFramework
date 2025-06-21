// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_WidgetIcon.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_WidgetIcon : public UFarmItemFragment
{
	GENERATED_BODY()

public:

	TSoftObjectPtr<UTexture2D> GetIcon() const {return Icon;};
	
private:

	UPROPERTY(EditAnywhere, Category="Item")
	TSoftObjectPtr<UTexture2D> Icon{nullptr};
	
};
