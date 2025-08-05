// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Farm_TraceComponent.h"

#include "Interface/Farm_HighlightableInterface.h"


UFarm_TraceComponent::UFarm_TraceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UFarm_TraceComponent::BeginPlay()
{
    Super::BeginPlay();


}

void UFarm_TraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TraceForwardForActor();
}

void UFarm_TraceComponent::TraceForwardForActor()
{
    if (!GEngine || !GetWorld())
    {
        return;
    }

    AActor* Owner = GetOwner();

    if (!Owner)
    {
        return;
    }

    //TODO : 무기 장착했는지 체크, 무기에서 범위 갖고오기

    FHitResult HitResult;
    FVector StartLocation = Owner->GetActorLocation();
    FVector EndLocation = StartLocation + StartLocation.ForwardVector * 100;
    if (!GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel))
    {
        return;
    }

    LastActor = ThisActor;
    ThisActor = HitResult.GetActor();

    if (ThisActor == LastActor)
    {
        return;
    }

    if (ThisActor.IsValid())
    {
        if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UFarm_HighlightableInterface::StaticClass()); IsValid(Highlightable))
        {
            IFarm_HighlightableInterface::Execute_Highlight(Highlightable);
        }
    }

    if (LastActor.IsValid())
    {
        if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UFarm_HighlightableInterface::StaticClass()); IsValid(Highlightable))
        {
            IFarm_HighlightableInterface::Execute_Unhighlight(Highlightable);
        }
    }
}

