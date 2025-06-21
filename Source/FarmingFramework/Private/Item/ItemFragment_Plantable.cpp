// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_Plantable.h"

#include "Components/ProgressionComponent.h"
#include "Components/Building/BuildingManagerComponent.h"
#include "FarmingFramework/Widgets/Farm_MouseHoverItem.h"
#include "Item/ItemFragment_ItemCount.h"
#include "Item/ItemFragment_MouserHoverWidgetCreate.h"
#include "Item/Components/Farm_ItemComponent.h"
#include "Kismet/GameplayStatics.h"

void UItemFragment_Plantable::OnInteract()
{
	if (!IsValid(SeedActorClass))
	{
		return;
	}
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);

	if (!IsValid(PC))
	{
		return;
	}

	UBuildingManagerComponent* BCM = PC->FindComponentByClass<UBuildingManagerComponent>();
	if (!IsValid(BCM))
	{
		return;
	}

	BCM->BuildStart(SeedActorClass);
	BCM->OnFinishPlacement.AddDynamic(this, &ThisClass::StartProgression);
}

void UItemFragment_Plantable::StartProgression(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	if (UProgressionComponent* ProgressionComponent = Actor->FindComponentByClass<UProgressionComponent>())
	{
		ProgressionComponent->Interact();
	}

	if (!IsValid(GetOwner()))
	{
		return;
	}
	const TWeakObjectPtr<UFarm_ItemComponent> ItemComponent = GetOwner()->FindComponentByClass<UFarm_ItemComponent>();
	if (!ItemComponent.IsValid())
	{
		return;
	}

	const UItemFragment_MouserHoverWidgetCreate* MouserHoverWidgetFragment = ItemComponent->GetFragment<UItemFragment_MouserHoverWidgetCreate>();
	if (!IsValid(MouserHoverWidgetFragment))
	{
		return;
	}


	if (UItemFragment_ItemCount* ItemCount_Fragment = ItemComponent->GetFragmentOfTypeMutable<UItemFragment_ItemCount>())
	{
		int32 NewCount = ItemCount_Fragment->GetCount() - 1;
		ItemCount_Fragment->SetCount(NewCount);

		MouserHoverWidgetFragment->GetMouseHoverWidget()->SetCount(NewCount);
	}

	
}
