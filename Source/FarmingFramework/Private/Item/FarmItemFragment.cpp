// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Item\FarmItemFragment.h"

#include "EngineUtils.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Interface/FarmItemFragmentInterface.h"
#include "Kismet/GameplayStatics.h"

bool UFarmItemFragment::bIsInteractBlocked = false;

void UFarmItemFragment::PostInitProperties()
{
	UObject::PostInitProperties();

	FunctionTag = GetFunctionTag();
}

AActor* UFarmItemFragment::GetOwner() const
{
	// 컴포넌트 -> 액터
	return Cast<AActor>(GetOuter()->GetOuter());
}

ACharacter* UFarmItemFragment::GetOwnerCharacter() const
{

	return UGameplayStatics::GetPlayerCharacter(this,0);
}

bool UFarmItemFragment::DetectFrontActor(float InTileDistance, AActor*& DetectedActor, const FGameplayTag& Tag, const TArray<AActor*>& InIgnoreActors)
{
	AActor* OwnerCharacter = GetOwnerCharacter();

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error , TEXT("No OwenrActor"));
		return false;
	}

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector End = Start + Forward * InTileDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.AddIgnoredActors(InIgnoreActors);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

#if 0
	DrawDebugLine(GetWorld(),Start,End,FColor::Red);
#endif

    if (DoesActorHaveTag(HitResult.GetActor(), Tag))
    {
        DetectedActor = HitResult.GetActor();
        return true;
    }

    DetectedActor = nullptr;

	return false;
}

bool UFarmItemFragment::DetectDownActor(float InTileDistance, AActor*& DetectedActor, const FGameplayTag& Tag, const TArray<AActor*>& InIgnoreActors)
{
    AActor* OwnerCharacter = GetOwnerCharacter();

    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error , TEXT("No OwenrActor"));
        return false;
    }

    FVector Up = OwnerCharacter->GetActorUpVector();
    FVector Start = OwnerCharacter->GetActorLocation() + Up * TileDistance;
    FVector End = OwnerCharacter->GetActorLocation() - Up * TileDistance;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);
    Params.AddIgnoredActors(InIgnoreActors);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        Params
    );

#if 0
    DrawDebugLine(GetWorld(),Start,End,FColor::Red);
#endif


    if (DoesActorHaveTag(HitResult.GetActor(), Tag))
    {
        DetectedActor = HitResult.GetActor();
        return true;
    }

    DetectedActor = nullptr;

    return false;
}

bool UFarmItemFragment::CheckFrontActorTagMatch(float InTileDistance, AActor*& DetectedActor, FGameplayTag InFunctionTag, const TArray<AActor*>& InIgnoreActors)
{
	AActor* Actor = nullptr;
	if(!DetectFrontActor(InTileDistance,Actor, InFunctionTag, InIgnoreActors))
	{
		return false;
	}

	if(!IsValid(Actor))
	{
		return false;
	}

	DetectedActor = Actor;

	//받은 인자가 없을 시 변수로 가지고 있는 태그로 설정해줍니다.
	if(!InFunctionTag.IsValid())
	{
		InFunctionTag = FunctionTag;
	}

	// 받은 인자 X 변수로 설정된 태그 X
	if(!InFunctionTag.IsValid())
	{
		//UE_LOG(LogTemp, Error, TEXT("CheckFrontActorTagMatch failed: InFunctionTag is not valid."));
		return false;
	}

    if (DoesActorHaveTag(Actor,InFunctionTag))
    {
        return true;
    }

	return false;
}

AActor* UFarmItemFragment::GetInteractableActor()
{
    AActor* InteractableActor;
    if (DetectFrontActor(TileDistance,InteractableActor , GetFunctionTag()))
    {
        return InteractableActor;
    }

    return nullptr;
}

bool UFarmItemFragment::DoesActorHaveTag(AActor* InActor, const FGameplayTag& InTag)
{
    if(IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(InActor))
    {
        FGameplayTagContainer ActorTag;
        TagInterface->GetOwnedGameplayTags(ActorTag);

        for(const FGameplayTag Tag : ActorTag)
        {
            if(Tag.MatchesTagExact(InTag))
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

#if 0
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

const UFarmItemFragment* UFarmItemStatics::FindItemActorFragment(TSubclassOf<AActor> ItemActor, TSubclassOf<UFarmItemFragment> FragmentClass)
{
	if (!ItemActor || !FragmentClass)
	{
		return nullptr;
	}

	// Get CDO (Class Default Object) to access default properties
	const AActor* CDO = ItemActor->GetDefaultObject<AActor>();
	if (!CDO)
	{
		return nullptr;
	}

	if(const IFarmItemFragmentInterface* FarmItemFragmentInterface = Cast<IFarmItemFragmentInterface>(CDO))
	{
		return FarmItemFragmentInterface->GetFarmItemFragment(FragmentClass);
	}

	return nullptr;
}
