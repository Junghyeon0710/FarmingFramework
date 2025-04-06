// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/ProgressionManagerComponent.h"

UProgressionManagerComponent::UProgressionManagerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

float UProgressionManagerComponent::Interact()
{
	return 0;
}


