// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_FootStep.h"

#include "Components/FootStepComponent.h"
#include "GameFramework/Character.h"


void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
    if (!IsValid(Character))
    {
        return;
    }

    if (UFootStepComponent* FootStepComponent = Character->FindComponentByClass<UFootStepComponent>())
    {
        FootStepComponent->FootStep(Character, SocketName, VolumeScale, VfxScale);
    }
}
