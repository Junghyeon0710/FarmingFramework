// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Farm_TraceComponent.h"

#include "Interface/Farm_HighlightableInterface.h"
#include "Item/FarmItemFragment.h"
#include "Item/Components/Farm_ItemComponent.h"


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

    ItemComponent = nullptr;
    TArray<AActor*> AttachedActors;
    GetOwner()->GetAttachedActors(AttachedActors);

    for (AActor* Attached : AttachedActors)
    {
        ItemComponent = Attached->FindComponentByClass<UFarm_ItemComponent>();
    }

    // if (UFarm_ItemComponent* FoundComponent = Owner->FindComponentByClass<UFarm_ItemComponent>())
    // {
    //     ItemComponent = FoundComponent;
    // }

    if (!ItemComponent.Get())
    {
        return;
    }


    FHitResult HitResult;
    FVector StartLocation = Owner->GetActorLocation();
    FVector EndLocation = StartLocation + Owner->GetActorForwardVector() * 100;
    GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, TraceChannel);


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
            for (const auto& Fragment : ItemComponent->GetFragments())
            {
                if (IFarm_HighlightableInterface::Execute_GetHighlightableTags(Highlightable).HasTagExact(Fragment->GetFunctionTag()))
                {
                    IFarm_HighlightableInterface::Execute_Highlight(Highlightable);
                }
            }
        }
    }

    if (LastActor.IsValid())
    {
        if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UFarm_HighlightableInterface::StaticClass()); IsValid(Highlightable))
        {
            IFarm_HighlightableInterface::Execute_Unhighlight(Highlightable);
        }
    }
}

