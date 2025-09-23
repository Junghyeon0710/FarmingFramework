// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemFragment_GroundLeveling.h"

#include "NativeGameplayTags.h"
#include "GameFramework/Character.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Ground_Flatten, "Interact.Ground.Leveling");

bool UItemFragment_GroundLeveling::OnInteract()
{
    if (!GetInteractableActor())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is not a farmland area"));
        return false;
    }

    AActor* DetectedActor = nullptr;
    if (DetectDownActor(TileDistance, DetectedActor, GetFunctionTag()))
    {
        if (DoesActorHaveTag(DetectedActor, IgnoreDestroyTag))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Placed Seed"));
            return false;
        }

        DetectedActor->Destroy();
        return true;
    }

    PlayMontage();
    return true;
}

FGameplayTag UItemFragment_GroundLeveling::GetFunctionTag() const
{
	return Interact_Ground_Flatten;
}

AActor* UItemFragment_GroundLeveling::GetInteractableActor()
{
    AActor* OwnerCharacter = GetOwnerCharacter();

    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error , TEXT("No OwnerActor"));
        return nullptr;
    }

    FVector Up = OwnerCharacter->GetActorUpVector();
    FVector Start = OwnerCharacter->GetActorLocation() + Up * TileDistance;
    FVector End = OwnerCharacter->GetActorLocation() - Up * TileDistance;

    FHitResult HitResult;
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, GroundChannel, FCollisionQueryParams());

    return bHit ? HitResult.GetActor() : nullptr;
}

void UItemFragment_GroundLeveling::OnMontageEnd()
{
    Super::OnMontageEnd();
    SpawnGroundActor();
}

void UItemFragment_GroundLeveling::SpawnGroundActor()
{
    if (!GroundActorClass)
    {
        return;
    }

    FVector OwnerLocation = GetOwner()->GetActorLocation();
    FVector SnappedLocation;
    SnappedLocation.X = FMath::GridSnap(OwnerLocation.X, GridRange);
    SnappedLocation.Y = FMath::GridSnap(OwnerLocation.Y, GridRange);
    SnappedLocation.Z = OwnerLocation.Z;

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    GetWorld()->SpawnActor<AActor>(GroundActorClass, SnappedLocation, FRotator::ZeroRotator, SpawnParameters);
}


