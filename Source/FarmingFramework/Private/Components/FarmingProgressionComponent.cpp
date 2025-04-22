// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FarmingProgressionComponent.h"
#include "../FarmingGameplayTag.h"

UFarmingProgressionComponent::UFarmingProgressionComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	
}

bool UFarmingProgressionComponent::CanChangeInitState(UProgressionComponentManager* Manager, FGameplayTag CurrentState,FGameplayTag DesiredState) const
{
	if(!Manager) return false;

	if(CurrentState == FarmingGameplayTags::Test_1 && DesiredState == FarmingGameplayTags::Test_2)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_1 && DesiredState == FarmingGameplayTags::Test_2"));
		return bTest1;
	}

	if(CurrentState == FarmingGameplayTags::Test_2 && DesiredState == FarmingGameplayTags::Test_3)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_2 && DesiredState == FarmingGameplayTags::Test_3"));
		return bTest2;
	}

	if(CurrentState == FarmingGameplayTags::Test_3 && DesiredState == FarmingGameplayTags::Test_4)
	{
		UE_LOG(LogTemp,Warning, TEXT("FarmingGameplayTags::Test_3 && DesiredState == FarmingGameplayTags::Test_4"));
		return bTest3;
	}

	return false;
}

void UFarmingProgressionComponent::HandleChangeInitState(UProgressionComponentManager* Manager,FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	
}

void UFarmingProgressionComponent::HandleFailedInitStateChange(UProgressionComponentManager* Manager,FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	UE_LOG(LogTemp,Log, TEXT("HandleFailedInitStateChange"));
}

