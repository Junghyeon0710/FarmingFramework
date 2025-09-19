// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FarmingProgressionComponent.h"

#include "FarmingFramework.h"
#include "../FarmingGameplayTag.h"
#include "NativeGameplayTags.h"
#include "Actors/FarmTaggedActor.h"
#include "Actors/InteractableActors/Tiles/Farm_TileActor.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Progress_01, "Progress.01");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Progress_02, "Progress.02");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Progress_03, "Progress.03");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Progress_04, "Progress.04");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Progress_05, "Progress.05");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Interact_Crop_Harvest, "Interact.Crop.Harvest");


UFarmingProgressionComponent::UFarmingProgressionComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

bool UFarmingProgressionComponent::CanChangeInitState(FGameplayTag CurrentState,FGameplayTag DesiredState) const
{

	if(CurrentState == Progress_01 && DesiredState == Progress_02)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_1 && DesiredState == FarmingGameplayTags::Test_2"));
		return true;
	}

	if(CurrentState == Progress_02 && DesiredState == Progress_03)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_2 && DesiredState == FarmingGameplayTags::Test_3"));
		return true;
	}

	if(CurrentState == Progress_03 && DesiredState == Progress_04)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_3 && DesiredState == FarmingGameplayTags::Test_4"));
		return true;
	}

    if(CurrentState == Progress_04 && DesiredState == Progress_05)
    {
        UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_4 && DesiredState == FarmingGameplayTags::Test_5"));
        return true;
    }

	return false;
}

void UFarmingProgressionComponent::HandleChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
    AActor* Seed = GetOwner();
    if (!IsValid(Seed))
    {
        return;;
    }

    AActor* Tile = Seed->GetOwner();
    if (!IsValid(Tile))
    {
        return;
    }

    if(CurrentState == Progress_01 && DesiredState == Progress_02)
    {
        UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_1 && DesiredState == FarmingGameplayTags::Test_2"));
        return;
    }

    if(CurrentState == Progress_02 && DesiredState == Progress_03)
    {
        UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_2 && DesiredState == FarmingGameplayTags::Test_3"));
        return;
    }

    if(CurrentState == Progress_03 && DesiredState == Progress_04)
    {
        UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_3 && DesiredState == FarmingGameplayTags::Test_4"));
        return;
    }

    // 끝까지 성장 했으므로 수확 가능 태그 부여
    if(CurrentState == Progress_04 && DesiredState == Progress_05)
    {
        if (AFarmTaggedActor* TaggedActor = Cast<AFarmTaggedActor>(Tile))
        {
            TaggedActor->AddStaticGameplayTag(Interact_Crop_Harvest);
        }
    }
}

void UFarmingProgressionComponent::HandleFailedInitStateChange(FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
    AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
	    UE_LOG(FarmLog, Error, TEXT("Failed to get valid owner in %s"), *GetName());
	    return;
	}

   UStaticMeshComponent* OwnerSMC = Owner->FindComponentByClass<UStaticMeshComponent>();
    if (!OwnerSMC)
    {
        UE_LOG(FarmLog, Warning, TEXT("UStaticMeshComponent not found in owner '%s' (%s)"), *Owner->GetName(), *Owner->GetClass()->GetName())
        return;
    }

    if (!FailedMesh)
    {
        UE_LOG(FarmLog, Warning, TEXT("Not Setting FailedMesh"));
    }
    OwnerSMC->SetStaticMesh(FailedMesh.LoadSynchronous());

}



