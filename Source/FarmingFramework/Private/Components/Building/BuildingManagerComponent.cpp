// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Building/BuildingManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UBuildingManagerComponent::UBuildingManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UBuildingManagerComponent::BuildStart(const TSubclassOf<AActor>& TargetClass)
{
	check(TargetClass);
	SpawnActor = GetWorld()->SpawnActor(TargetClass);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PC->SetShowMouseCursor(true);
	StartBuildTimer();
	
}

void UBuildingManagerComponent::StartBuildTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BuildTimer,this,&ThisClass::UpdateBuildAsset,0.01,true,0);
}

bool UBuildingManagerComponent::GetTraceHitResult(FHitResult& Result,ECollisionChannel TraceChannel)
{
	bool bHit = false;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FVector MouseWorldLocation;
	FVector MouseWorldDirection;

	if (PC && PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
	{
		FVector TraceStart = MouseWorldLocation;
		FVector TraceEnd = MouseWorldLocation + (MouseWorldDirection * 100000.f);
		
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(SpawnActor);
		bHit = GetWorld()->LineTraceSingleByChannel(
			Result,
			TraceStart,
			TraceEnd,
			TraceChannel,
			Params
		);
	}
	
	return  bHit;
}

bool UBuildingManagerComponent::GetTraceHitResult(FHitResult& Result, const TArray<AActor*>& IgnoreActors,ECollisionChannel TraceChannel)
{
	bool bHit = false;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FVector MouseWorldLocation;
	FVector MouseWorldDirection;

	if (PC && PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
	{
		FVector TraceStart = MouseWorldLocation;
		FVector TraceEnd = MouseWorldLocation + (MouseWorldDirection * 100000.f);
		
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.AddIgnoredActors(IgnoreActors);
		bHit = GetWorld()->LineTraceSingleByChannel(
			Result,
			TraceStart,
			TraceEnd,
			TraceChannel,
			Params
		);
	}
	
	return  bHit;
}

bool UBuildingManagerComponent::IsCornersCheck()
{
	if (!SpawnActor) return false;

	FVector Origin, BoxExtent;
	SpawnActor->GetActorBounds(false, Origin, BoxExtent);

	// 확장 범위 적용
	float HalfWidth = BoxExtent.X + ExtraRange;
	float HalfHeight = BoxExtent.Y + ExtraRange;

	// 코너 위치 계산
	FVector TopRight = Origin + FVector(HalfWidth, HalfHeight, 0);
	FVector TopLeft = Origin + FVector(-HalfWidth, HalfHeight, 0);
	FVector BottomRight = Origin + FVector(HalfWidth, -HalfHeight, 0);
	FVector BottomLeft = Origin + FVector(-HalfWidth, -HalfHeight, 0);
	
	FHitResult OutHit;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(SpawnActor);

	// 각 코너에 대해 트레이스 실행
	return PerformLineTrace(TopRight, IgnoreActors, OutHit)
		&& PerformLineTrace(TopLeft, IgnoreActors, OutHit)
		&& PerformLineTrace(BottomRight, IgnoreActors, OutHit)
		&& PerformLineTrace(BottomLeft, IgnoreActors, OutHit);
}

bool UBuildingManagerComponent::PerformLineTrace(const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, FHitResult& OutHit,ECollisionChannel TraceChannel /* = ECC_Visibility */)
{
	FVector TraceStart = StartPoint + FVector(0.f, 0.f, 100.f); 
	FVector TraceEnd = StartPoint + FVector(0.f, 0.f, -1.f);    
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors); 
	QueryParams.bTraceComplex = true; 
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,         
		TraceStart,     
		TraceEnd,       
		TraceChannel,   
		QueryParams     
	);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, bHit ? FColor::Green : FColor::Red, false, 1.f, 0, 1.f);
#endif
	if(bHit)
	{
		if(HitClassType)
		{
			bHit = !OutHit.GetActor()->IsA(HitClassType); 
		}
		else
		{
			bHit = !OutHit.GetActor()->IsA(AActor::StaticClass()); // 히트한게 액터 클래스인지?
		}
	}
	return bHit; 
}

FVector UBuildingManagerComponent::GridPosition(const FVector& InParam) const
{
	const FVector DivideVector =  InParam/GridValue;
	
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * GridValue,UKismetMathLibrary::Round(DivideVector.Y) * GridValue, 40.f );
}

void UBuildingManagerComponent::UpdateBuildAsset()
{
	FHitResult HitResult;
	if (GetTraceHitResult(HitResult))
	{
		// 마우스가 가리키는 실제 지점
		FVector HitLocation = HitResult.Location;

		// 100cm (1m) 간격으로 스냅
		FVector SnappedLocation = HitLocation.GridSnap(GridValue);
		// FVector SpawnOrigin;
		// FVector SpawnBoxExtent;
		//SpawnActor->GetActorBounds(false,SpawnOrigin,SpawnBoxExtent);
		FVector Origin, BoxExtent;
		SpawnActor->GetActorBounds(true, Origin, BoxExtent); // 충돌 기준

		// Z축 보정: 액터의 반높이만큼 위로 올리기

		SnappedLocation.Z = HitLocation.Z + BoxExtent.Z;
		// 액터 위치 설정
		if (SpawnActor)
		{
			SpawnActor->SetActorLocation(SnappedLocation);
		}

		// 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,FString::Printf(TEXT("Hit: %s"), *HitLocation.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,FString::Printf(TEXT("Snapped: %s"), *SpawnActor->GetActorLocation().ToString()));

		bCanDrop = false;
	}

	//코너 반경 확인
	bCanDrop =  IsCornersCheck; 
	
	
	//머티리얼 설정
	UStaticMeshComponent* MeshComponent = SpawnActor->FindComponentByClass<UStaticMeshComponent>();
	check(MeshComponent);
	
	MeshComponent->SetMaterial(0, bCanDrop ? CanBuildMaterial : CanNotBuildMaterial);
}

