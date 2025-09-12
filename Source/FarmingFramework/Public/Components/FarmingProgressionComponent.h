// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressionComponent.h"
#include "Interface/ProgressionStateInterface.h"
#include "FarmingProgressionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFarmingProgressionComponent : public UProgressionComponent , public IProgressionStateInterface
{
	GENERATED_BODY()

public:
	UFarmingProgressionComponent();

	/** IProgressionStateInterface*/
	virtual bool CanChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleFailedInitStateChange(FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	/** ~IProgressionStateInterface */

protected:

public:



	//Field 레벨 관리
	UPROPERTY(EditAnywhere, Category = Filed)
	int32 FieldLevel;

	//Field 등급 관리
	UPROPERTY(EditAnywhere, Category = Filed)
	int32 FieldGrade;

	//물뿌리개 사용 여부 관리
	UPROPERTY(EditAnywhere, Category = "Field")
	bool bIsWatered;

	//시들어가는 식물 단계 관리
	UPROPERTY(EditAnywhere, Category = "Field")
	int32 WitheredStage;

	// Plane액터의 텍스처 관리
	UPROPERTY(EditAnywhere, Category = "Field")
	UTexture2D* FieldPlaneTexture;

	//seed/ 심은 작물 관리

	// 파종 여부 관리
	UPROPERTY(EditAnywhere, Category = "Field")
	bool bIsSown;

    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UStaticMesh> FailedMesh;
};
