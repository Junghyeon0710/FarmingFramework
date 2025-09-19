// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProgressionCompletionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UProgressionCompletionInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class FARMINGFRAMEWORK_API IProgressionCompletionInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual void SetCompletedClass(TSoftClassPtr<AActor> CompletedClassCompletedClass) {};
    virtual TSoftClassPtr<AActor> GetCompletedClass() {return nullptr;}
};
