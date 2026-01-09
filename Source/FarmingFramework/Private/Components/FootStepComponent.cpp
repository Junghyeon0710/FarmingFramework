// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FootStepComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UFootStepComponent::UFootStepComponent()
{

    PrimaryComponentTick.bCanEverTick = false;

}

void UFootStepComponent::FootStep(const ACharacter* InCharacter, const FName& InSocketName, float InVolumeScale, float InVfxScale)
{
    if (!InCharacter || OwnerCharacter != InCharacter)
    {
        return;
    }

    const USkeletalMeshComponent* Mesh = InCharacter->GetMesh();
    if (!Mesh)
    {
        return;
    }

    constexpr float TraceHalfHeight = 40.f;

    const FVector FootStepLocation = Mesh->GetSocketLocation(InSocketName);
    const FVector StartLocation = FootStepLocation + FVector::UpVector * TraceHalfHeight;
    const FVector EndLocation   = FootStepLocation - FVector::UpVector * TraceHalfHeight;

    FHitResult HitResult;
    if (!GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
    {
        return;
    }

    const FName PhysMatName = HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->GetFName() : TEXT("Default");

    FFootStepInfo* FootStepInfo = GetFootStepInfo(PhysMatName);
    if (!FootStepInfo)
    {
        return;
    }

    const FVector SpawnLocation = HitResult.ImpactPoint;

    // VFX
    if (FootStepInfo->FootStepVfx)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),FootStepInfo->FootStepVfx, SpawnLocation, InCharacter->GetActorRotation(),FVector(InVfxScale));
    }

    // Sound
    if (FootStepInfo->FootStepSound)
    {
        if (UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(),FootStepInfo->FootStepSound, SpawnLocation,FRotator::ZeroRotator, InVolumeScale))
        {
            AudioComponent->SetIntParameter(TEXT("SurfaceType"), FootStepInfo->FootStepType);
        }
    }
}

FFootStepInfo* UFootStepComponent::GetFootStepInfo(const FName& InPhysicsMaterialName)
{
    if (FFootStepInfo* FootStepInfo = FootStepInfos.Find(InPhysicsMaterialName))
    {
        return FootStepInfo;
    }

    return nullptr;
}

void UFootStepComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
}


