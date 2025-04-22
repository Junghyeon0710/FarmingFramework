// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "TileItemBase.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API ATileItemBase : public AActor , public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	ATileItemBase();

	/** IGameplayTagAssetInterface*/
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/** ~IGameplayTagAssetInterface*/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor)
	FGameplayTagContainer StaticGameplayTags;
};
