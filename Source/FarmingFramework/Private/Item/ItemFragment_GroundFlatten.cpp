// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_GroundFlatten.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Character.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Ground_Flatten, "Interact.Ground.Flatten");

void UItemFragment_GroundFlatten::OnInteract()
{
    if (GetInteractableActor())
    {
        SpawnGroundActor();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Ground Actor"));
    }
}

void UItemFragment_GroundFlatten::OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor)
{
	if(InFunctionTag.MatchesTagExact(GetFunctionTag()))
	{
		// 애니메이션

		//땅 고르기 행동
		UE_LOG(LogTemp,Warning,TEXT("%s"),*GetFunctionTag().ToString());

	}
}

FGameplayTag UItemFragment_GroundFlatten::GetFunctionTag() const
{
	return Interact_Ground_Flatten;
}

AActor* UItemFragment_GroundFlatten::GetInteractableActor()
{
    AActor* OwnerCharacter = GetOwnerCharacter();

    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error , TEXT("No OwenrActor"));
        return nullptr;
    }

    FVector Up = OwnerCharacter->GetActorUpVector();
    FVector Start = OwnerCharacter->GetActorLocation() + Up * TileDistance;
    FVector End = OwnerCharacter->GetActorLocation() - Up * TileDistance;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Channel, FCollisionQueryParams());

    return HitResult.GetActor();
}

void UItemFragment_GroundFlatten::SpawnGroundActor()
{
    if (!GroundActorClass)
    {
        return;
    }

    FVector OwnerLocation = GetOwner()->GetActorLocation();

    // 100 단위 그리드로 스냅
    FVector SnappedLocation;
    SnappedLocation.X = FMath::GridSnap(OwnerLocation.X, 100.f);
    SnappedLocation.Y = FMath::GridSnap(OwnerLocation.Y, 100.f);
    SnappedLocation.Z = OwnerLocation.Z; // Z는 그대로 두거나, 필요하면 따로 스냅

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    GetWorld()->SpawnActor<AActor>(
        GroundActorClass,
        SnappedLocation,
        FRotator::ZeroRotator,
        SpawnParameters
    );
}


