// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Farm_MouseHoverItem.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;
/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UFarm_MouseHoverItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIcon(const TSoftObjectPtr<UTexture2D> Icon) const;
	void SetCount(const int32 InCount);

	int32 GetItemCount() const {return ItemCount;};
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	int32 ItemCount;
};
