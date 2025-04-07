// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/ProgressionManagerComponent.h"

UProgressionManagerComponent::UProgressionManagerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

float UProgressionManagerComponent::Interact()
{
	float Delay;
	int32 Stage;
	FarmingProgress(Delay,Stage);
	return Delay;

	// AActor* Actor = GetOwner();
	// UStaticMeshComponent* ab=  Actor->FindComponentByClass<UStaticMeshComponent>();
	// ab->SetStaticMesh(as);
}

void UProgressionManagerComponent::SetProgressionState(float Progression)
{
}

void UProgressionManagerComponent::FarmingProgress(float& OutDelay, int32& Stage)
{
}

void UProgressionManagerComponent::SwitchStage()
{
}

void UProgressionManagerComponent::SetReady()
{
}


