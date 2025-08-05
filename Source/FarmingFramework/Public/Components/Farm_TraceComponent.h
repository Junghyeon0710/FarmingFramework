// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Farm_TraceComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFarm_TraceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFarm_TraceComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void TraceForwardForActor();


    UPROPERTY(EditAnywhere)
    TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

    TWeakObjectPtr<AActor> ThisActor;
    TWeakObjectPtr<AActor> LastActor;
};
