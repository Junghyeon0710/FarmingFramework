// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FarmItemFragment.h"
#include "ItemFragment_MouserHoverWidgetCreate.generated.h"

class UFarm_MouseHoverItem;
/**
 * 
 */
UCLASS()
class FARMINGFRAMEWORK_API UItemFragment_MouserHoverWidgetCreate : public UFarmItemFragment
{
	GENERATED_BODY()

public:
	UFarm_MouseHoverItem* GetMouseHoverWidget()const {return MouserHoverItem;}
	
protected:
	virtual void OnFragmentCreated() override;


private:

	UPROPERTY(EditAnywhere, Category = Class)
	TSubclassOf<UFarm_MouseHoverItem> MouserHoverItemClass;
	
	UPROPERTY()
	TObjectPtr<UFarm_MouseHoverItem> MouserHoverItem;
};
