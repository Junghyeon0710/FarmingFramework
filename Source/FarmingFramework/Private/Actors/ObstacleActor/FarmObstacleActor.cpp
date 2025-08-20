// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObstacleActor/FarmObstacleActor.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/Interaction/Farm_HighlightableStaticMesh.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"


AFarmObstacleActor::AFarmObstacleActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;


	{
		Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
		SetRootComponent(Collision);

		// Collision->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		// Collision->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
	}

	HighlightableMesh = CreateDefaultSubobject<UFarm_HighlightableStaticMesh>(TEXT("HighlightableMesh"));
	HighlightableMesh->SetupAttachment(RootComponent);
}

void AFarmObstacleActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if ( PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFarmObstacleActor, RemoveMeshList))
    {
        RandomMeshList = RemoveMeshList;
    }
}

void AFarmObstacleActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (!bUseRandomMesh || RandomMeshList.IsEmpty())
    {
        return;
    }

    if (!HighlightableMesh)
    {
       return;
    }

    HighlightableMesh->SetStaticMesh(RandomMeshList[FMath::RandRange(0,RandomMeshList.Num()-1)].LoadSynchronous());

    if (!HighlightableMesh->GetStaticMesh() || !Collision)
    {
        return;
    }

    FBoxSphereBounds MeshBound = HighlightableMesh->GetStaticMesh()->GetBounds();
    Collision->SetBoxExtent(MeshBound.BoxExtent * 2);

    UpdateRequiredInteractionsFromHighlightMesh();
}

void AFarmObstacleActor::Interact(AActor* Interactor)
{
    OnInteract(Interactor);
}

void AFarmObstacleActor::OnInteract(AActor* Interactor)
{
    if (!Interactor)
    {
        return;
    }

    CurrentInteractionCount++;

    if (CurrentInteractionCount >= RequiredInteractions)
    {
        CurrentInteractionCount = 0;
        Destroy();
        return;
    }

    LoadAndSetMeshAsync();
}

void AFarmObstacleActor::LoadAndSetMeshAsync()
{
    if (RemoveMeshList.IsValidIndex(CurrentInteractionCount))
    {
        if (RemoveMeshList[CurrentInteractionCount].Get())
        {
            HighlightableMesh->SetStaticMesh(RemoveMeshList[CurrentInteractionCount].Get());
        }
        else
        {
            TSoftObjectPtr<UStaticMesh> SoftMesh = RemoveMeshList[CurrentInteractionCount];

            if (!SoftMesh.IsNull())
            {
                FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

                Streamable.RequestAsyncLoad(SoftMesh.ToSoftObjectPath(),
                  FStreamableDelegate::CreateLambda([this, SoftMesh]()
                  {
                      if (HighlightableMesh && SoftMesh.IsValid())
                      {
                          HighlightableMesh->SetStaticMesh(SoftMesh.Get());
                      }
                  })
                );
            }
        }
    }
}

void AFarmObstacleActor::UpdateRequiredInteractionsFromHighlightMesh()
{
    if (RemoveMeshList.IsEmpty() || !HighlightableMesh)
    {
        return;
    }

    UStaticMesh* CurrentMesh = HighlightableMesh->GetStaticMesh();
    if (!CurrentMesh)
    {
        return;
    }

    FSoftObjectPath CurrentMeshPath(CurrentMesh);

    for (int32 i = 0; i < RemoveMeshList.Num(); ++i)
    {
        if (RemoveMeshList[i].ToSoftObjectPath() == CurrentMeshPath)
        {
            RequiredInteractions =  RemoveMeshList.Num() - i;
            return;
        }
    }
}

// void AFarmObstacleActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if(OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
// 	{
// 		//Show UI
// 		//UE_LOG(LogTemp,Warning,TEXT("%s : BeginOverlap"),*OtherActor->GetName());
// 	}
//
// }
//
// void AFarmObstacleActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	if(OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
// 	{
// 		//HIde UI
// 		//UE_LOG(LogTemp,Warning,TEXT("%s : EndOverlap"),*OtherActor->GetName());
//
// 	}
// }






