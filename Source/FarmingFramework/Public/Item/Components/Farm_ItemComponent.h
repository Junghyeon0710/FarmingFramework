// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Farm_ItemComponent.generated.h"


class UFarmItemFragment;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FARMINGFRAMEWORK_API UFarm_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFarm_ItemComponent();

	static UFarm_ItemComponent* FindItemComponent(const AActor* OwnerActor) {return OwnerActor->FindComponentByClass<UFarm_ItemComponent>();}

	virtual UFarmItemFragment* GetFarmItemFragment(TSubclassOf<UFarmItemFragment> FragmentClass) const ;

	template< typename FragmentType> requires std::derived_from<FragmentType, UFarmItemFragment>
	const FragmentType* GetFragment();

	template<typename FragmentType> requires std::derived_from<FragmentType, UFarmItemFragment>
	FragmentType* GetFragmentOfTypeMutable();

	UFUNCTION(BlueprintCallable)
	void TryUseItem();
protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(Instanced,EditAnywhere, Category = "Item")
	TArray<TObjectPtr<UFarmItemFragment>> Fragments;
};

template< typename FragmentType> requires std::derived_from<FragmentType, UFarmItemFragment>
const FragmentType* UFarm_ItemComponent::GetFragment()
{
	for (const UFarmItemFragment* Fragment : Fragments)
	{
		if (const FragmentType* CastedFragment = Cast<FragmentType>(Fragment))
		{
			return CastedFragment;
		}
	}

	return nullptr;
}

template <typename FragmentType> requires std::derived_from<FragmentType, UFarmItemFragment>
FragmentType* UFarm_ItemComponent::GetFragmentOfTypeMutable()
{
	for (UFarmItemFragment* Fragment : Fragments)
	{
		if (FragmentType* CastedFragment = Cast<FragmentType>(Fragment))
		{
			return CastedFragment;
		}
	}

	return nullptr;
}
