// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_MouserHoverWidgetCreate.h"

#include "FarmingFramework/Widgets/Farm_MouseHoverItem.h"
#include "GameFramework/PlayerController.h"
#include "Item/ItemFragment_ItemCount.h"
#include "Item/ItemFragment_WidgetIcon.h"
#include "Item/Components/Farm_ItemComponent.h"
#include "Kismet/GameplayStatics.h"

void UItemFragment_MouserHoverWidgetCreate::OnFragmentCreated()
{
	if (!IsValid(MouserHoverItemClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("MouserHoverItemClass is not set"));
		return;
	}
	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not set"));
		return;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetOwner(),0);
	
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is not set"));
		return;
	}

	if (!IsValid(MouserHoverItem))
	{
		MouserHoverItem = CreateWidget<UFarm_MouseHoverItem>(PC, MouserHoverItemClass);
	}

	TWeakObjectPtr<UFarm_ItemComponent> ItemComponent = GetOwner()->FindComponentByClass<UFarm_ItemComponent>();
	if (ItemComponent.IsValid())
	{
		const UItemFragment_ItemCount* Count = ItemComponent->GetFragment<UItemFragment_ItemCount>();
		const UItemFragment_WidgetIcon* Icon = ItemComponent->GetFragment<UItemFragment_WidgetIcon>();

		if (IsValid(Count) && IsValid(Icon))
		{
			MouserHoverItem->SetIcon(Icon->GetIcon());
			MouserHoverItem->SetCount(Count->GetCount());
		}
	}

	PC->SetShowMouseCursor(true);
	PC->SetMouseCursorWidget(EMouseCursor::Default, MouserHoverItem);
}
