// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ProgressionStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProgressionStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMINGFRAMEWORK_API IProgressionStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** 원하는 상태로 도달할 수 있는지 체크  */
	virtual bool CanChangeInitState( FGameplayTag CurrentState, FGameplayTag DesiredState) const {return true;}
};
