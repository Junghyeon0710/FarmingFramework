// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFinishPlacement,AActor*, SpawnedActor);

UENUM(BlueprintType)
enum class ECheckMode : uint8
{
	Overlap         UMETA(DisplayName = "Overlap"), /** 액터 기준으로 오버랩 체크합니다.*/
	LineTrace       UMETA(DisplayName = "Line Trace"), 	/** 마우스 기준으로 히트 체크합니다.*/
	MultiLineTrace  UMETA(DisplayName = "Multi Line Trace") 	/** 마우스 기준으로 오버랩,히트 체크합니다.*/
};

UENUM(BlueprintType)
enum class EPivotPosition : uint8
{
	Center UMETA(DisplayName = "Center"),
	Bottom UMETA(DisplayName = "Bottom")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UBuildingManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UBuildingManagerComponent();
#if  WITH_EDITOR
	virtual void PostInitProperties() override; 
#endif
	
	UFUNCTION(BlueprintCallable)
	void BuildStart(const TSubclassOf<AActor>& SpawnClass, const TSubclassOf<AActor>& TargetClass = nullptr);
	UFUNCTION(BlueprintCallable)
    void FinishPlacement();
    
	void StartBuildTimer();
	bool GetTraceHitResult(FHitResult& Result,ECollisionChannel TraceChannel = ECC_Visibility);
	bool GetTraceHitResult(FHitResult& Result,const TArray<AActor*>& IgnoreActors ,ECollisionChannel TraceChannel = ECC_Visibility);
	bool IsCornersCheck();
	bool PerformLineTrace(	const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, TArray<FHitResult>& OutHits,ECollisionChannel TraceChannel = ECC_Visibility );


	UPROPERTY(BlueprintAssignable)
	FFinishPlacement OnFinishPlacement;
public:
	
	FVector GridPosition(const FVector& InParam) const;
	EPivotPosition GetActorPivotPosition(const AActor* Actor);
	EPivotPosition GetStaticMeshPivotPosition(const UStaticMeshComponent* MeshComp);
	EPivotPosition GetSkeletalMeshPivotPosition(const USkeletalMeshComponent* MeshComp);
public:
	bool IsClassInHitList(AActor* Actor);
	bool CheckLineTrace();
	bool CheckOverlap();
	bool CheckMultiLineTrace();

protected:

	UPROPERTY(EditAnywhere,Category = Check)
	ECheckMode CheckMode = ECheckMode::Overlap;
	
	//UPROPERTY(EditAnywhere,Category = Check)
	EPivotPosition PivotPivotPosition = EPivotPosition::Bottom;
protected:
	virtual void UpdateBuildAsset();

	UPROPERTY(EditAnywhere,Category = Gird)
	int32 GridValue = 100;
	UPROPERTY(EditAnywhere,Category = Gird)
	float ExtraRange = 100.f;

	UPROPERTY(EditAnywhere,Category = Gird)
	TObjectPtr<UMaterialInterface> CanBuildMaterial;
	UPROPERTY(EditAnywhere,Category = Gird)
	TObjectPtr<UMaterialInterface> CanNotBuildMaterial;

	UPROPERTY(EditAnywhere,Category = Gird)
	TArray<TSubclassOf<AActor>> HitClassTypes;

	UPROPERTY(EditAnywhere,Category = Gird)
	TEnumAsByte<ECollisionChannel> CollisionChannel;
private:
	bool bCanDrop = true;

	UPROPERTY(VisibleAnywhere,Category = Spanw)
	TObjectPtr<AActor> SpawnActor;

	TWeakObjectPtr<AActor> LastSpawnedActor;
	TWeakObjectPtr<AActor> TargetActor;
	TWeakObjectPtr<AActor> LastTargetActor;

	FTimerHandle BuildTimer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInterface> ActorMaterial;
	
};
