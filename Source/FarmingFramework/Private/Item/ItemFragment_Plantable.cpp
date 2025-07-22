// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_Plantable.h"

#include "Actors/InteractableActors/Tiles/Farm_TileActor.h"
#include "Components/ProgressionComponent.h"
#include "FarmingFramework/Widgets/Farm_MouseHoverItem.h"
#include "Item/ItemFragment_ItemCount.h"
#include "Item/ItemFragment_MouserHoverWidgetCreate.h"
#include "Item/Components/Farm_ItemComponent.h"
#include "Kismet/GameplayStatics.h"

// void UItemFragment_Plantable::OnInteract()
// {
// 	if (!IsValid(SeedActorClass))
// 	{
// 		return;
// 	}
//
// 	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
//
// 	if (!IsValid(PC))
// 	{
// 		return;
// 	}
//
// 	UBuildingManagerComponent* BCM = PC->FindComponentByClass<UBuildingManagerComponent>();
// 	if (!IsValid(BCM))
// 	{
// 		return;
// 	}
//
// 	const TWeakObjectPtr<UFarm_ItemComponent> ItemComponent = GetOwner()->FindComponentByClass<UFarm_ItemComponent>();
// 	if (!ItemComponent.IsValid())
// 	{
// 		return;
// 	}
//
// 	if (UItemFragment_ItemCount* ItemCount_Fragment = ItemComponent->GetFragmentOfTypeMutable<UItemFragment_ItemCount>())
// 	{
// 		if (ItemCount_Fragment->GetCount() <= 0)
// 		{
// 			return;
// 		}
// 	}
//
// 	BCM->BuildStart(SeedActorClass,AFarm_TileActor::StaticClass());
//
// 	if (!BCM->OnFinishPlacement.IsAlreadyBound(this, &ThisClass::StartProgression))
// 	{
// 		BCM->OnFinishPlacement.AddDynamic(this, &ThisClass::StartProgression);
// 	}
// }

// void UItemFragment_Plantable::StartProgression(AActor* Actor)
// {
// 	if (!IsValid(Actor))
// 	{
// 		return;
// 	}
//
// 	if (UProgressionComponent* ProgressionComponent = Actor->FindComponentByClass<UProgressionComponent>())
// 	{
// 		ProgressionComponent->Interact();
// 	}
//
// 	if (!IsValid(GetOwner()))
// 	{
// 		return;
// 	}
//
// 	const TWeakObjectPtr<UFarm_ItemComponent> ItemComponent = GetOwner()->FindComponentByClass<UFarm_ItemComponent>();
// 	if (!ItemComponent.IsValid())
// 	{
// 		return;
// 	}
//
// 	const UItemFragment_MouserHoverWidgetCreate* MouserHoverWidgetFragment = ItemComponent->GetFragment<UItemFragment_MouserHoverWidgetCreate>();
// 	if (!IsValid(MouserHoverWidgetFragment))
// 	{
// 		return;
// 	}
//
//
// 	if (UItemFragment_ItemCount* ItemCount_Fragment = ItemComponent->GetFragmentOfTypeMutable<UItemFragment_ItemCount>())
// 	{
// 	    PlayMontage();
// 		int32 NewCount = ItemCount_Fragment->GetCount() - 1;
// 		ItemCount_Fragment->SetCount(NewCount);
//
// 		MouserHoverWidgetFragment->GetMouseHoverWidget()->SetCount(NewCount);
//
// 		if(NewCount > 0)
// 		{
// 			OnInteract();
// 		}
// 	}
// }
void UItemFragment_Plantable::OnInteractSuccess(AActor*& DetectedActor)
{
    if (!IsValid(SeedActorClass))
    {
        return;
    }

    PlayMontage();

	FVector SpawnLocation;
    EActorPivotPosition PivotPosition = GetActorPivotPosition(DetectedActor);

    UStaticMeshComponent* MeshComp = DetectedActor->FindComponentByClass<UStaticMeshComponent>();
    if (MeshComp && MeshComp->GetStaticMesh())
    {
        const FVector ActorLocation = DetectedActor->GetActorLocation();
        const FBox LocalBounds = MeshComp->GetStaticMesh()->GetBoundingBox();
        const FVector BoxExtent = LocalBounds.GetExtent();

        // Z 오프셋 계산
        float ZOffset = 0.f;
        switch (PivotPosition)
        {
        case EActorPivotPosition::Center:
            ZOffset = BoxExtent.Z * MeshComp->GetComponentScale().Z;
            break;

        case EActorPivotPosition::Bottom:
            ZOffset = BoxExtent.Z * 2.f * MeshComp->GetComponentScale().Z;
            break;

        default:
            break;
        }

        SpawnLocation = ActorLocation + FVector(0.f, 0.f, ZOffset);
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* Seed = GetWorld()->SpawnActor<AActor>(SeedActorClass, SpawnLocation, DetectedActor->GetActorRotation() ,Params);


    if (!Seed)
    {
        return;
    }

    // 진행
    if (UProgressionComponent* ProgressionComponent = Seed->FindComponentByClass<UProgressionComponent>())
    {
        ProgressionComponent->Interact();
    }

    //감지 액터에 배치 알림
    if (DetectedActor->Implements<UBuildTargetInterface>())
    {
        IBuildTargetInterface::Execute_OnBuildingPlaced(DetectedActor, Seed);
    }

}

EActorPivotPosition UItemFragment_Plantable::GetActorPivotPosition(const AActor* Actor)
{
    if (UStaticMeshComponent* StaticMeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>())
    {
        return GetStaticMeshPivotPosition(StaticMeshComponent);
    }

    if (USkeletalMeshComponent* SkeletalMeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>())
    {
        return GetSkeletalMeshPivotPosition(SkeletalMeshComponent);
    }

    return EActorPivotPosition::Bottom;
}

EActorPivotPosition UItemFragment_Plantable::GetStaticMeshPivotPosition(const UStaticMeshComponent* MeshComp)
{
    if (MeshComp && MeshComp->GetStaticMesh())
    {
        // 메쉬의 로컬 공간에서 AABB 중심 위치
        const FBox LocalBounds = MeshComp->GetStaticMesh()->GetBoundingBox();
        const FVector Center = LocalBounds.GetCenter();
        const FVector Min = LocalBounds.Min;

        // 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
        float CenterZ = Center.Z;
        float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
        float MinZ = Min.Z;

        // 어느 쪽에 가까운지 판단
        float DistToCenter = FMath::Abs(PivotZ - CenterZ);
        float DistToBottom = FMath::Abs(PivotZ - MinZ);

        if (DistToCenter < DistToBottom)
        {
            return EActorPivotPosition::Center;
        }
    }
    return EActorPivotPosition::Bottom;
}

EActorPivotPosition UItemFragment_Plantable::GetSkeletalMeshPivotPosition(const USkeletalMeshComponent* MeshComp)
{
    if (MeshComp && MeshComp->GetSkeletalMeshAsset())
    {
        // 메쉬의 로컬 공간에서 AABB 중심 위치
        const FBox LocalBounds = MeshComp->GetSkeletalMeshAsset()->GetImportedBounds().GetBox();
        const FVector Center = LocalBounds.GetCenter();
        const FVector Min = LocalBounds.Min;

        // 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
        float CenterZ = Center.Z;
        float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
        float MinZ = Min.Z;

        // 어느 쪽에 가까운지 판단
        float DistToCenter = FMath::Abs(PivotZ - CenterZ);
        float DistToBottom = FMath::Abs(PivotZ - MinZ);

        if (DistToCenter < DistToBottom)
        {
            return EActorPivotPosition::Center;
        }
    }
    return EActorPivotPosition::Bottom;
}
