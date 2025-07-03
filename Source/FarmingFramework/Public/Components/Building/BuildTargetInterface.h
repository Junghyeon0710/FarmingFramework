// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuildTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBuildTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMINGFRAMEWORK_API IBuildTargetInterface
{
	GENERATED_BODY()

public:

	/** 빌딩이 성공적으로 배치되었을 때 호출 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBuildingPlaced(AActor* PlacedActor);

	/** 여기 위에 지워도 되는가 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanBeBuiltOn();
};
