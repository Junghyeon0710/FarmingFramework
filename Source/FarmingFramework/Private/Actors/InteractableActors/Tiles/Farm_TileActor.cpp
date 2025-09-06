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

AActor* AFarm_TileActor::CheckAdjacentTile(const FVector& Direction) const
{
    FHitResult HitResult;

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + Direction * 100, ECC_Visibility);
    return bHit ? HitResult.GetActor() : nullptr;
}

bool AFarm_TileActor::CheckLeftTile()
{
   LeftTile = CheckAdjacentTile(-GetActorRightVector());

   return LeftTile ? true : false;
}

bool AFarm_TileActor::CheckRightTile()
{
    RightTile = CheckAdjacentTile(GetActorRightVector());
    return RightTile ? true : false;
}


