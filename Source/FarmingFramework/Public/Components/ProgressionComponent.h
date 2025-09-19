// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "Components/ActorComponent.h"
#include "ProgressionComponent.generated.h"

class UStaticMesh;
class USkeletalMeshComponent;


USTRUCT(BlueprintType)
struct FMeshList
{
	GENERATED_BODY()
    FMeshList() :Mesh(nullptr), TransitionTime(0.f), ProgressionState(){}

    FMeshList(UStaticMesh* InMesh, float InTransitionTime, const FGameplayTag& InProgressionState)
        : Mesh(InMesh)
        , TransitionTime(InTransitionTime)
        , ProgressionState(InProgressionState){}

	// UPROPERTY(EditAnywhere)
	// TObjectPtr<USkeletalMesh> Mesh;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMesh> Mesh;

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
public:
	FGameplayTag GetCurrentState();
	FGameplayTag GetDesiredState();

	void RegisterInitTags(const TArray<FGameplayTag>& InInitTags);
	void UnRegisterInitTags();

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = List)
	TArray<FMeshList> MeshList;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = List)
    TSoftClassPtr<AActor> CompletedClass;

	/** 진행 상태*/
	int32 ProgressState = 0;

	UPROPERTY()
	USkeletalMeshComponent* NewSkeletalMeshComp = nullptr;

    UPROPERTY()
    UStaticMeshComponent* NewStaticMeshComp = nullptr;

	UPROPERTY()
	TArray<FGameplayTag> InitTags;

	/** Tag 상태*/
	int32 InitState = 0;

public:
    FORCEINLINE void SetMeshLists(const TArray<FMeshList>& InMeshLists) { MeshList = InMeshLists; }
    FORCEINLINE const TArray<FMeshList>& GetMeshLists() { return MeshList; }
    FORCEINLINE void SetCompletedClass(const TSoftClassPtr<AActor>& InCompletedClass) { CompletedClass = InCompletedClass; }
    FORCEINLINE TSoftClassPtr<AActor> GetCompletedClass() { return CompletedClass; }
};
