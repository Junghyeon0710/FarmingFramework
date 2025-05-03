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
	FVector GridPosition(const FVector& InParam,int32 GridValue) const;
protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

private:


	UPROPERTY(VisibleAnywhere,Category = Spanw)
	TObjectPtr<AActor> SpawnActor;

	bool bIsSpawn = false;
	
};
