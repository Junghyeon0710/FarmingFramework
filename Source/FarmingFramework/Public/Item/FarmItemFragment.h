// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FarmItemFragment.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced,EditInlineNew,Abstract ,Blueprintable, BlueprintType)
class FARMINGFRAMEWORK_API UFarmItemFragment : public UObject
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract() {};
	UFUNCTION(BlueprintCallable)
	virtual void OnInteractWithTag(FGameplayTag InFunctionTag) {}; // 태그 기반
	UFUNCTION(BlueprintCallable)
	virtual void OnInteractWithActor(FGameplayTag InFunctionTag, AActor* DetectedActor) {}; // 태그 + 대상 액터


	/** 멤버변수로 태그설정 안했으면 재정의해서 태그 설정을 해주세요. */
	virtual FGameplayTag GetFunctionTag() const {return FunctionTag;}
	
public:

	AActor* GetOwner() const;
	
	ACharacter* GetOwnerCharacter() const;
	template<typename T>
	T* GetOwnerCharacter() const
	{
		return Cast<T>(GetOwnerCharacter());
	}
	
public:
	
	/**
	 * Owner 기준 앞에 액터가 있는지 감지하는 함수
	 * @param TileDistance    캐릭터 앞 방향 기준 거리
	 * @param DetectedActor   감지된 액터
	 * @return 
	 */
	bool DetectFrontActor(float TileDistance,AActor*& DetectedActor);

	/**
	 * Owner 기준 앞에 액터의 태그가 일치하는지 체크 함수
	 * @param TileDistance 캐릭터 앞 방향 기준 거리
	 * @param InFunctionTag 찾고싶은 태그 (없을시 변수로 설정한 태그 가져옵니다)
	 * @return 
	 */
	bool CheckFrontActorTagMatch(float TileDistance ,FGameplayTag InFunctionTag = FGameplayTag());


	bool operator==(const FGameplayTag& Other) const {return  this->FunctionTag.MatchesTagExact(Other);}
	//bool operator!=(const FGameplayTag& Other) const {return !operator==(Other);}
protected:
	
	UPROPERTY(EditAnywhere)
	FGameplayTag FunctionTag;
	
	UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> FuncTObjectionTag;
};

UCLASS()
class FARMINGFRAMEWORK_API UFarmItemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FGameplayTagContainer TraceForwardForActorTagContainer(AActor* SourceActor,float TraceDistance, ECollisionChannel Channel = ECC_Visibility);

	UFUNCTION(BlueprintPure)
	static FGameplayTag TraceForwardForActorFirstTag(AActor* SourceActor,float TraceDistance, ECollisionChannel Channel = ECC_Visibility);

	UFUNCTION(BlueprintPure)
	static FGameplayTag TraceForwardForActorFirstTagWithActor(AActor*& DetectActor,AActor* SourceActor,float TraceDistance, ECollisionChannel Channel = ECC_Visibility);

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UFarmItemFragment* FindItemActorFragment(TSubclassOf<AActor> ItemActor, TSubclassOf<UFarmItemFragment> FragmentClass);
};