// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "ProgressionStateInterface.generated.h"

class UProgressionComponentManager;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProgressionStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMINGFRAMEWORK_API IProgressionStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Returns the Actor this object is bound to, might be this object */
	virtual AActor* GetOwningActor() const;

	//virtual void RegisterInitTags(const TArray<FGameplayTag>& Tags);

	virtual bool ContinueInitStateChain(FGameplayTag CurrentState,FGameplayTag DesiredState);
	
	/** */
	virtual void BindOnActorInitStateChanged();
	
	/** 원하는 상태로 도달할 수 있는지 체크  */
	virtual bool CanChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const {return true;}

	/** 성공시 호출 함수(추가 기능 구현)  */
	virtual void HandleChangeInitState(FGameplayTag CurrentState, FGameplayTag DesiredState) const {};
	
	/** 실패시 호출 함수(추가 기능 구현)  */
	virtual void HandleFailedInitStateChange(FGameplayTag CurrentState, FGameplayTag DesiredState) const {};
	
};
