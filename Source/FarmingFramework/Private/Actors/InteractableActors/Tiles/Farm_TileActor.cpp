// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActors/Tiles/Farm_TileActor.h"
#include "NativeGameplayTags.h"
#include "DynamicWeatherSubsystem.h"
#include "Components/BoxComponent.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Ignore_Received_Water, "Ignore.Received.Water");
UE_DEFINE_GAMEPLAY_TAG_STATIC(Ignore_Placed_Seed, "Ignore.Placed.Seed");

AFarm_TileActor::AFarm_TileActor(const FObjectInitializer& ObjectInitializer)
{

}

void AFarm_TileActor::BeginPlay()
{
	Super::BeginPlay();

	UDynamicWeatherSubsystem* Subsystem = GetWorld()->GetSubsystem<UDynamicWeatherSubsystem>();
	if (IsValid(Subsystem))
	{
		Subsystem->OnDayChanged.AddDynamic(this, &ThisClass::OnDayChange);
		if (Subsystem->IsRaining())
		{
			ApplyWetSoilVisual();
		}
	}
}

void AFarm_TileActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    ConnectRidgeWithNeighbors();

}

void AFarm_TileActor::Destroyed()
{
    if (LeftTile.IsValid())
    {
        LeftTile->RefreshAdjacentRidges();
    }
    if (RightTile.IsValid())
    {
        RightTile->RefreshAdjacentRidges();
    }

    Super::Destroyed();
}

void AFarm_TileActor::OnDayChange(int32 Year, int32 Day, const FString& Season, EWeatherType Weather)
{
	if (Weather == EWeatherType::Rain || Weather == EWeatherType::Snow)
	{
		ApplyWetSoilVisual();
	}
}

void AFarm_TileActor::OnInteract()
{
	ApplyWetSoilVisual();
}

bool AFarm_TileActor::TryCropHarvest(float LifeSpan)
{
    if (!GetSeed())
    {
        return false;
    }

    RemoveStaticGameplayTag(Ignore_Placed_Seed);
    GetSeed()->SetLifeSpan(LifeSpan);
    OnCropHarvest();
    return true;
}

void AFarm_TileActor::OnBuildingPlaced_Implementation(AActor* PlacedActor)
{
	if (!IsValid(PlacedActor))
	{
		return;
	}
	SetSeed(PlacedActor);
    PlacedActor->SetOwner(this);
	AddStaticGameplayTag(Ignore_Placed_Seed);

}

bool AFarm_TileActor::CanBeBuiltOn_Implementation()
{
	return !IsValid(Seed);
}

void AFarm_TileActor::ApplyWetSoilVisual()
{
	InitializeDynamicMaterial();

	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue(MultiplyColor_Wet, MultiplyColor_LinearColor);
	}

	AddStaticGameplayTag(Ignore_Received_Water);
//	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECR_Overlap);
}

void AFarm_TileActor::InitializeDynamicMaterial()
{
	if (!DynMaterial && Mesh)
	{
		DynMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	}
}

AFarm_TileActor* AFarm_TileActor::CheckAdjacentTile(const FVector& Direction) const
{
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + Direction * 50, ECC_Visibility, Params);

    AFarm_TileActor* Tile = Cast<AFarm_TileActor>(HitResult.GetActor());

    return bHit && IsValid(Tile) && !Tile->IsPendingKillPending() ? Tile : nullptr;
}

bool AFarm_TileActor::CheckLeftTile()
{
   LeftTile = CheckAdjacentTile(-GetActorRightVector());

   return IsValid(LeftTile.Get());
}

bool AFarm_TileActor::CheckRightTile()
{
    RightTile = CheckAdjacentTile(GetActorRightVector());
    return IsValid(RightTile.Get());
}

void AFarm_TileActor::ConnectRidgeWithNeighbors()
{
    RefreshAdjacentRidges();

    if (LeftTile.IsValid())
    {
        LeftTile->RefreshAdjacentRidges();
    }
    if (RightTile.IsValid())
    {
        RightTile->RefreshAdjacentRidges();
    }
}

void AFarm_TileActor::RefreshAdjacentRidges()
{
    const bool bHasLeft  = CheckLeftTile();
    const bool bHasRight = CheckRightTile();

    UStaticMesh* SelectedMesh = TileMesh;

    if (bHasLeft && bHasRight)
    {
        SelectedMesh = TileMesh_M;
    }
    else if (bHasLeft)
    {
        SelectedMesh = TileMesh_R;
    }
    else if (bHasRight)
    {
        SelectedMesh = TileMesh_L;
    }

    Mesh->SetStaticMesh(SelectedMesh);
}


