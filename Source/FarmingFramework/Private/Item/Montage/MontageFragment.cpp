// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Montage/MontageFragment.h"

#include "GameFramework/Character.h"

void UMontageFragment::PlayMontage()
{
	if (!AnimMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("No Montage"));
		return;
	}
	
	if (UAnimInstance* AnimInstance = GetOwnerCharacter()->GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AnimMontage, 1.0f);
	}
}
