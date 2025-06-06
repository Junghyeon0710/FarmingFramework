﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FarmItemFragmentInterface.generated.h"

class UFarmItemFragment;

// This class does not need to be modified.
UINTERFACE()
class UFarmItemFragmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMINGFRAMEWORK_API IFarmItemFragmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//virtual TArray<UFarmItemFragment*> GetFarmItemFragments() const = 0;

	virtual UFarmItemFragment* GetFarmItemFragment(TSubclassOf<UFarmItemFragment> FragmentClass) const = 0;
};
