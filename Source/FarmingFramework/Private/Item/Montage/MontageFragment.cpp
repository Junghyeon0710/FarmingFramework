// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Montage/MontageFragment.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMontageFragment::PlayMontage()
{
	if (!AnimMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("No Montage"));
		return;
	}

	if (UAnimInstance* AnimInstance = GetOwnerCharacter()->GetMesh()->GetAnimInstance())
	{
		MontageLength = AnimInstance->Montage_Play(AnimMontage, 1.0f);
	    GetOwnerCharacter()->GetCharacterMovement()->DisableMovement();

	    FOnMontageEnded OnMontageEnded;
	    OnMontageEnded.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
        {
            if (!bInterrupted)
            {
                OnMontageEnd();
            }
        });

	    AnimInstance->Montage_SetEndDelegate(OnMontageEnded, AnimMontage);
	}
}

void UMontageFragment::OnMontageEnd()
{
    GetOwnerCharacter()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
