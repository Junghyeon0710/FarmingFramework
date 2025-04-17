// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ProgressionComponent.generated.h"

class UStaticMesh;
class USkeletalMeshComponent;


USTRUCT(BlueprintType)
struct FMeshList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere)
	float TransitionTime;

	UPROPERTY(EditAnywhere)
	FGameplayTag ProgressionState;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UProgressionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProgressionComponent();

	UFUNCTION(BlueprintCallable)
	virtual void Interact();
	
	FTimerHandle ProgressTimer;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = List)
	TArray<FMeshList> MeshList;

	int32 ProgressState =0;

	UPROPERTY()
	USkeletalMeshComponent* NewStaticMeshComp = nullptr;
};
