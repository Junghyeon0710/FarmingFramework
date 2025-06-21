// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmingFramework/Widgets/Farm_MouseHoverItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UFarm_MouseHoverItem::SetIcon(const TSoftObjectPtr<UTexture2D> Icon) const
{
	Image_Icon->SetBrushFromSoftTexture(Icon);
}

void UFarm_MouseHoverItem::SetCount(const int32 InCount)
{
	Text_Count->SetText(FText::AsNumber(InCount));
	ItemCount = InCount;
}
