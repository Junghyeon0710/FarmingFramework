// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObstacleActor/FarmObstacleActor.h"

#include "Components/BoxComponent.h"
#include "Components/Interaction/Farm_HighlightableStaticMesh.h"
#include "Engine/AssetManager.h"


AFarmObstacleActor::AFarmObstacleActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	{
		Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
		Collision->SetupAttachment(RootComponent);

	    Collision->SetCollisionObjectType(ECC_Vehicle);
	    Collision->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	    Collision->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	    Collision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		// Collision->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnBeginOverlap);
		// Collision->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnEndOverlap);
	}

	HighlightableMesh = CreateDefaultSubobject<UFarm_HighlightableStaticMesh>(TEXT("HighlightableMesh"));
	HighlightableMesh->SetupAttachment(RootComponent);
   // SetRootComponent(HighlightableMesh);
}

void AFarmObstacleActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if ( PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFarmObstacleActor, RemoveMeshList))
    {
        RandomMeshList = RemoveMeshList;
    }

    if ( PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFarmObstacleActor, RandomMeshList) && RemoveMeshList.Num() > 0)
    {
        RandomMeshList = RemoveMeshList;
        UE_LOG(LogTemp, Warning, TEXT("RemoveMeshList exists. Do not set RandomMeshList."));
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

    if (!HighlightableMesh->GetStaticMesh())
    {
        return;
    }

    FBoxSphereBounds MeshBound = HighlightableMesh->GetStaticMesh()->GetBounds();
    Collision->SetBoxExtent(MeshBound.BoxExtent * 1.1);
    UpdateRequiredInteractionsFromHighlightMesh();

    if (HighlightableMesh && HighlightableMesh->GetStaticMesh())
    {
        // 메쉬의 로컬 공간에서 AABB 중심 위치
        const FBox LocalBounds = HighlightableMesh->GetStaticMesh()->GetBoundingBox();
        const FVector Center = LocalBounds.GetCenter();
        const FVector Min = LocalBounds.Min;

        // 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
        float CenterZ = Center.Z;
        float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
        float MinZ = Min.Z;

        // 어느 쪽에 가까운지 판단
        float DistToCenter = FMath::Abs(PivotZ - CenterZ);
        float DistToBottom = FMath::Abs(PivotZ - MinZ);

        if (DistToCenter < DistToBottom)
        {
            FVector NewLocation = GetActorLocation();
            NewLocation.Z += MeshBound.BoxExtent.Z;
            SetActorLocation(NewLocation);
        }
    }
}

void AFarmObstacleActor::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, GetClass());

    if (OverlappingActors.Num() > 0)
    {
        Destroy();
    }
}

void AFarmObstacleActor::Interact(AActor* Interactor)
{
    OnInteract(Interactor);
}

void AFarmObstacleActor::FinishSpawn()
{
    FBoxSphereBounds MeshBound = HighlightableMesh->GetStaticMesh()->GetBounds();
    if (HighlightableMesh && HighlightableMesh->GetStaticMesh())
    {
        // 메쉬의 로컬 공간에서 AABB 중심 위치
        const FBox LocalBounds = HighlightableMesh->GetStaticMesh()->GetBoundingBox();
        const FVector Center = LocalBounds.GetCenter();
        const FVector Min = LocalBounds.Min;

        // 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
        float CenterZ = Center.Z;
        float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
        float MinZ = Min.Z;

        // 어느 쪽에 가까운지 판단
        float DistToCenter = FMath::Abs(PivotZ - CenterZ);
        float DistToBottom = FMath::Abs(PivotZ - MinZ);

        if (DistToCenter < DistToBottom)
        {
            FVector NewLocation = GetActorLocation();
            NewLocation.Z += MeshBound.BoxExtent.Z;
            SetActorLocation(NewLocation);
        }
    }

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, GetClass());

    if (OverlappingActors.Num() > 0)
    {
        Destroy();
    }
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






