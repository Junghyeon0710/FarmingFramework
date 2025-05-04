// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UBuildingManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UBuildingManagerComponent();

	UFUNCTION(BlueprintCallable)
	void BuildStart(const TSubclassOf<AActor>& TargetClass);
	void StartBuildTimer();
	bool GetTraceHitResult(FHitResult& Result,ECollisionChannel TraceChannel = ECC_Visibility);
	bool GetTraceHitResult(FHitResult& Result,const TArray<AActor*>& IgnoreActors ,ECollisionChannel TraceChannel = ECC_Visibility);
	bool IsCornersCheck();
	bool PerformLineTrace(	const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, FHitResult& OutHit,ECollisionChannel TraceChannel = ECC_Visibility );
public:
	
	FVector GridPosition(const FVector& InParam) const;

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
	TSubclassOf<AActor> HitClassType;
private:
	bool bCanDrop;

	UPROPERTY(VisibleAnywhere,Category = Spanw)
	TObjectPtr<AActor> SpawnActor;

	FTimerHandle BuildTimer;
	
};
