// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObstacleActor/FarmObstacleActor.h"

#include "Components/SphereComponent.h"
#include "Components/Interaction/Farm_HighlightableStaticMesh.h"
#include "GameFramework/Character.h"


AFarmObstacleActor::AFarmObstacleActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;


	{
		Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
		SetRootComponent(Collision);

		Collision->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		Collision->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
	}

	HighlightableMesh = CreateDefaultSubobject<UFarm_HighlightableStaticMesh>(TEXT("HighlightableMesh"));
	HighlightableMesh->SetupAttachment(RootComponent);
}

void AFarmObstacleActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		//Show UI
		//UE_LOG(LogTemp,Warning,TEXT("%s : BeginOverlap"),*OtherActor->GetName());
	}

}

void AFarmObstacleActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		//HIde UI
		//UE_LOG(LogTemp,Warning,TEXT("%s : EndOverlap"),*OtherActor->GetName());

	}
}






