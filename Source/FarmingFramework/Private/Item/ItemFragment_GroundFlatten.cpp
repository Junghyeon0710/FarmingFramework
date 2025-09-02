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

    FVector Start = OwnerCharacter->GetActorLocation();
    FVector Up = OwnerCharacter->GetActorUpVector();
    FVector End = (Start + Up * TileDistance) - (Start - Up * TileDistance);

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

    GetWorld()->SpawnActor<AActor>(GroundActorClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
}


