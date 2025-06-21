// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Building/BuildingManagerComponent.h"

#include "Landscape.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UBuildingManagerComponent::UBuildingManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UBuildingManagerComponent::PostInitProperties()
{
	Super::PostInitProperties();
	
}

void UBuildingManagerComponent::BuildStart(const TSubclassOf<AActor>& TargetClass)
{
	check(TargetClass);
	SpawnActor = GetWorld()->SpawnActor(TargetClass);
	
	if (UStaticMeshComponent* StaticMeshComp = SpawnActor->FindComponentByClass<UStaticMeshComponent>())
	{
		 ActorMaterial = StaticMeshComp->GetMaterial(0); // 원래 머티리얼로 복구하고 싶다면 따로 저장해뒀다 복원
	}
	else if (USkeletalMeshComponent* SkeletalMeshComp = SpawnActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		 ActorMaterial = SkeletalMeshComp->GetMaterial(0);
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PC->SetShowMouseCursor(true);
	StartBuildTimer();
	
}

void UBuildingManagerComponent::StartBuildTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BuildTimer,this,&ThisClass::UpdateBuildAsset,0.05,true,0);
}

void UBuildingManagerComponent::UpdateBuildAsset()
{
	check(SpawnActor);
	FHitResult HitResult;
	bCanDrop = true;
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
		

		SnappedLocation.Z = HitLocation.Z + BoxExtent.Z;

		const FVector InterpVector = FMath::VInterpTo(SpawnActor->GetActorLocation(),SnappedLocation,GetWorld()->GetDeltaSeconds(),12.f);
		SpawnActor->SetActorLocation(SnappedLocation);
		
		if(CheckMode == ECheckMode::LineTrace)
		{
			bCanDrop = !CheckLineTrace();
		}
		else if(CheckMode == ECheckMode::Overlap)
		{
			bCanDrop = !CheckOverlap();
		}
		else if(CheckMode == ECheckMode::MultiLineTrace)
		{
			bCanDrop = !CheckMultiLineTrace();
		}
	}

	//코너 반경 확인
	if(bCanDrop)
	{
		bCanDrop =  IsCornersCheck(); 
	}
	
	//머티리얼 설정
	
	check(CanBuildMaterial && CanNotBuildMaterial);
	
	UStaticMeshComponent* MeshComponent = SpawnActor->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent)
	{
		MeshComponent->SetMaterial(0, bCanDrop ? CanBuildMaterial : CanNotBuildMaterial);
		return;
	}
	USkeletalMeshComponent* SkeletalMeshComponent = SpawnActor->FindComponentByClass<USkeletalMeshComponent>();
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetMaterial(0, bCanDrop ? CanBuildMaterial : CanNotBuildMaterial);
	}
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
	
	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(SpawnActor);

	// 각 코너에 대해 트레이스 실행
	return PerformLineTrace(TopRight, IgnoreActors, OutHits)
		&& PerformLineTrace(TopLeft, IgnoreActors, OutHits)
		&& PerformLineTrace(BottomRight, IgnoreActors, OutHits)
		&& PerformLineTrace(BottomLeft, IgnoreActors, OutHits);
}

bool UBuildingManagerComponent::PerformLineTrace(const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, TArray<FHitResult>& OutHits,ECollisionChannel TraceChannel /* = ECC_Visibility */)
{
	// LineTrace를 실행하여 주어진 점에서 아래로 충돌을 확인
	bool bHit =  UKismetSystemLibrary::LineTraceMultiByProfile(
		this,
		FVector(StartPoint.X, StartPoint.Y, 100),  // Z 값은 100으로 고정하여 위에서 아래로 트레이스
		FVector(StartPoint.X, StartPoint.Y, -1.f),  // -1로 Z 값을 낮추어 트레이스 진행
		"Visibility",  // 프로파일 이름은 "Visibility"로 설정
		false,          // 트레이스에서 충돌이 발생한 오브젝트는 무시하지 않음
		IgnoreActors,   // 트레이스에서 무시할 액터 리스트
		EDrawDebugTrace::None,  // 디버그 표시: 1 프레임 동안만 표시
		OutHits,        // 충돌 결과가 저장될 배열
		true            // 히트 결과를 캐스팅하여 처리
	);

#if 0
	DrawDebugLine(GetWorld(), FVector(StartPoint.X, StartPoint.Y, 100), FVector(StartPoint.X, StartPoint.Y, -1.f), bHit ? FColor::Green : FColor::Red, false, 1.f, 0, 1.f);
#endif
	if(bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			if (IsClassInHitList(Hit.GetActor()))
			{
				return false;
			}
		}
		return true;
	}
	//아무것도 없으면 배치되면 안됨,
	return false; 
}

void UBuildingManagerComponent::FinishPlacement()
{
	if (!SpawnActor || !bCanDrop)
	{
		// 배치 불가
		return;
	}

	// 빌드 타이머 정지
	GetWorld()->GetTimerManager().ClearTimer(BuildTimer);

	// 머티리얼 원상복구
	if (UStaticMeshComponent* StaticMeshComp = SpawnActor->FindComponentByClass<UStaticMeshComponent>())
	{
		StaticMeshComp->SetMaterial(0,ActorMaterial); // 원래 머티리얼로 복구하고 싶다면 따로 저장해뒀다 복원
	}
	else if (USkeletalMeshComponent* SkeletalMeshComp = SpawnActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		SkeletalMeshComp->SetMaterial(0, ActorMaterial);
	}

	// SpawnActor 고정화 처리 (필요하면)
	SpawnActor->SetActorEnableCollision(true);
	


	// 마우스 커서 끄고 싶다면
	// APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// if (PC)
	// {
	// 	PC->SetShowMouseCursor(false);
	// }
	OnFinishPlacement.Broadcast(SpawnActor);

	// 다음 빌드를 위해 null로 초기화
	SpawnActor = nullptr;
}

FVector UBuildingManagerComponent::GridPosition(const FVector& InParam) const
{
	const FVector DivideVector =  InParam/GridValue;
	
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * GridValue,UKismetMathLibrary::Round(DivideVector.Y) * GridValue, 40.f );
}

bool UBuildingManagerComponent::IsClassInHitList(AActor* Actor) const
{
	if (!Actor) return false;

	for (const TSubclassOf<AActor>& ClassType : HitClassTypes)
	{
		if (ClassType && Actor->IsA(ClassType))
		{
			return true;
		}
	}
	return false;
}

bool UBuildingManagerComponent::CheckLineTrace()
{
	FHitResult Result;
	FVector Start = SpawnActor->GetActorLocation() + FVector(0, 0, 100);
	FVector End = SpawnActor->GetActorLocation() + FVector(0, 0, -1000);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SpawnActor);

	if (GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, Params))
	{
		return IsClassInHitList(Result.GetActor());
	}
	return false;
}

bool UBuildingManagerComponent::CheckOverlap()
{
	TArray<AActor*> OverlappingActors;
	SpawnActor->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (IsClassInHitList(Actor))
		{
			return true;
		}
	}
	return false;
}

bool UBuildingManagerComponent::CheckMultiLineTrace()
{
	TArray<FHitResult> HitResults;
	FVector Start = SpawnActor->GetActorLocation() + FVector(0, 0, 100);
	FVector End = SpawnActor->GetActorLocation() + FVector(0, 0, -1000);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SpawnActor);

	if (GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, Params))
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (IsClassInHitList(Hit.GetActor()))
			{
				return true;
			}
		}
	}
	return false;
}



