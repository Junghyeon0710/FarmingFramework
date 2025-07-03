// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "FarmTaggedActor.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API AFarmTaggedActor : public AActor , public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	AFarmTaggedActor();

	/** IGameplayTagAssetInterface*/
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/** ~IGameplayTagAssetInterface*/

	void AddStaticGameplayTag(const FGameplayTag& NewTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor)
	FGameplayTagContainer StaticGameplayTags;
};
