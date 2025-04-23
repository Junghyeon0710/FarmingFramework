// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Item\FarmItemFragment.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

void UFarmItemFragment::PostInitProperties()
{
	UObject::PostInitProperties();

	FunctionTag = GetFunctionTag();
}

AActor* UFarmItemFragment::GetOwner() const
{
	return Cast<AActor>(GetOuter());
}

ACharacter* UFarmItemFragment::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOuter());
	
}

bool UFarmItemFragment::DetectFrontActor(float TileDistance,AActor*& DetectedActor)
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		UE_LOG(LogTemp, Error , TEXT("No OwenrActor"));
		return false;
	}

	FVector Start = Owner->GetActorLocation();
	FVector Forward = Owner->GetActorForwardVector();
	FVector End = Start + Forward * TileDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	
#if 1
	DrawDebugLine(GetWorld(),Start,End,FColor::Red);
#endif
	
	DetectedActor = HitResult.GetActor();
	return bHit;
}

bool UFarmItemFragment::CheckFrontActorTagMatch(float TileDistance,FGameplayTag InFunctionTag)
{
	AActor* Actor;
	if(!DetectFrontActor(TileDistance,Actor))
	{
		return false;
	}

	if(Actor == nullptr)
	{
		return false;
	}

	//받은 인자가 없을 시 변수로 가지고 있는 태그로 설정해줍니다.
	if(!InFunctionTag.IsValid())
	{
		InFunctionTag = FunctionTag;
	}
	
	// 받은 인자 X 변수로 설정된 태그 X
	if(!InFunctionTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CheckFrontActorTagMatch failed: InFunctionTag is not valid."));
		return false;
	}
	
	if(IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Actor))
	{
		FGameplayTagContainer ActorTag;
		TagInterface->GetOwnedGameplayTags(ActorTag);

		for(const FGameplayTag Tag : ActorTag)
		{
			if(Tag.MatchesTagExact(InFunctionTag))
			{
				return true;
			}
		}
	}

	return false;
}

FGameplayTagContainer UFarmItemStatics::TraceForwardForActorTagContainer(AActor* SourceActor, float TraceDistance , ECollisionChannel Channel)
{
	if(!SourceActor)
	{
		return FGameplayTagContainer();
	}

	FGameplayTagContainer Result ;
	
	FVector Start = SourceActor->GetActorLocation();
	FVector Forward = SourceActor->GetActorForwardVector();
	FVector End = Start + Forward * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SourceActor);
	
	bool bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		Channel,
		Params
	);

#if 1
	DrawDebugLine(SourceActor->GetWorld(),Start,End,FColor::Red,false,5.f);
#endif

	if(!HitResult.GetActor() || !bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No actor detected in front."));
		return FGameplayTagContainer();
	}

	if(IGameplayTagAssetInterface* TagInterface =  Cast<IGameplayTagAssetInterface>(HitResult.GetActor()))
	{
		TagInterface->GetOwnedGameplayTags(Result);
		return Result;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Actor does not implement IGameplayTagAssetInterface."));
	
	return Result;
}

 FGameplayTag UFarmItemStatics::TraceForwardForActorFirstTag(AActor* SourceActor, float TraceDistance,ECollisionChannel Channel)
{
	return TraceForwardForActorTagContainer(SourceActor,TraceDistance,Channel).First();
}

FGameplayTag UFarmItemStatics::TraceForwardForActorFirstTagWithActor(AActor*& DetectActor,AActor* SourceActor, float TraceDistance,ECollisionChannel Channel)
{
	if(!SourceActor)
	{
		return FGameplayTag();
	}

	FGameplayTagContainer Result ;
	
	FVector Start = SourceActor->GetActorLocation();
	FVector Forward = SourceActor->GetActorForwardVector();
	FVector End = Start + Forward * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SourceActor);
	
	bool bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		Channel,
		Params
	);

#if 1
	DrawDebugLine(SourceActor->GetWorld(),Start,End,FColor::Red,false,5.f);
#endif
	
	if(!HitResult.GetActor() || !bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No actor detected in front."));
		return FGameplayTag();
	}

	DetectActor = HitResult.GetActor();
	
	if(IGameplayTagAssetInterface* TagInterface =  Cast<IGameplayTagAssetInterface>(HitResult.GetActor()))
	{
		TagInterface->GetOwnedGameplayTags(Result);
		return Result.First();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Actor does not implement IGameplayTagAssetInterface."));

	// 2) 앞에 아무것도 없으면 → 지면 확인
	FVector GroundCheckStart = End + FVector(0,0,50);
	FVector GroundCheckEnd   = End - FVector(0,0,500);
	FHitResult GroundHit;
    
	bHit = SourceActor->GetWorld()->LineTraceSingleByChannel(
			GroundHit,
			GroundCheckStart,
			GroundCheckEnd,
			Channel,
			Params
			);

#if 1
	DrawDebugLine(SourceActor->GetWorld(),GroundCheckStart,GroundCheckEnd,FColor::Red,false,5.f);
#endif
	
		
	return FGameplayTag();
}
