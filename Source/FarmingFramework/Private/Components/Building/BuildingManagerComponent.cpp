// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Building/BuildingManagerComponent.h"

#include "Landscape.h"
#include "Components/Building/BuildTargetInterface.h"
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

void UBuildingManagerComponent::BuildStart(const TSubclassOf<AActor>& SpawnClass, const TSubclassOf<AActor>& TargetClass)
{
	check(SpawnClass);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnActor = GetWorld()->SpawnActor<AActor>(SpawnClass,Params);
	HitClassTypes.Add(TargetClass);
	
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
	FVector HitLocation;

	TargetActor = nullptr;
	
	if (GetTraceHitResult(HitResult))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (IsClassInHitList(HitActor))
			{
				EPivotPosition PivotPosition = GetActorPivotPosition(HitActor);

				UStaticMeshComponent* MeshComp = HitActor->FindComponentByClass<UStaticMeshComponent>();
				if (MeshComp && MeshComp->GetStaticMesh())
				{
					const FVector ActorLocation = HitActor->GetActorLocation();
					const FBox LocalBounds = MeshComp->GetStaticMesh()->GetBoundingBox();
					const FVector BoxExtent = LocalBounds.GetExtent();

					// Z 오프셋 계산
					float ZOffset = 0.f;
					switch (PivotPosition)
					{
					case EPivotPosition::Center:
						ZOffset = BoxExtent.Z * MeshComp->GetComponentScale().Z;
						break;

					case EPivotPosition::Bottom:
						ZOffset = BoxExtent.Z * 2.f * MeshComp->GetComponentScale().Z;
						break;

					default:
						break;
					}

					HitLocation = ActorLocation + FVector(0.f, 0.f, ZOffset);
				}
			}
			else
			{
				HitLocation = HitResult.Location;
			}
		}
		
		// 100cm (1m) 간격으로 스냅
		FVector SnappedLocation = HitLocation.GridSnap(GridValue);
		SnappedLocation.Z = HitLocation.Z;
		
		SpawnActor->SetActorLocation(SnappedLocation);
		
		
		if(CheckMode == ECheckMode::LineTrace)
		{
			bCanDrop = CheckLineTrace();
		}
		else if(CheckMode == ECheckMode::Overlap)
		{
			bCanDrop = CheckOverlap();
		}
		else if(CheckMode == ECheckMode::MultiLineTrace)
		{
			bCanDrop = CheckMultiLineTrace();
		}
	}

	//코너 반경 확인
	if(bCanDrop)
	{
	//	bCanDrop =  IsCornersCheck(); 
	}

	// 타겟 위에 지워도 되는지 체크
	if (TargetActor.IsValid())
	{
		if (TargetActor->Implements<UBuildTargetInterface>())
		{
			bCanDrop = IBuildTargetInterface::Execute_CanBeBuiltOn(TargetActor.Get());
		}
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
		//Params.bTraceComplex = true;
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
	FName ProfileName = UCollisionProfile::Get()->ReturnChannelNameFromContainerIndex(CollisionChannel);
	bool bHit =  UKismetSystemLibrary::LineTraceMultiByProfile(
		this,
		FVector(StartPoint.X, StartPoint.Y, 100),  // Z 값은 100으로 고정하여 위에서 아래로 트레이스
		FVector(StartPoint.X, StartPoint.Y, -1.f),  // -1로 Z 값을 낮추어 트레이스 진행
		ProfileName,  // 프로파일 이름은 "Visibility"로 설정
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

	//히트 액터한테 액터가 배치 됐다고 알려줌
	if (TargetActor.IsValid())
	{
		if (TargetActor->Implements<UBuildTargetInterface>())
		{
			IBuildTargetInterface::Execute_OnBuildingPlaced(TargetActor.Get(),TargetActor.Get());
		}
	}
	
	LastSpawnedActor = SpawnActor;
	OnFinishPlacement.Broadcast(SpawnActor);

	// Broadcast후 바로 SpawnActor 스폰, SpawnActor 삭제 방지
	if (LastSpawnedActor == SpawnActor)
	{
		SpawnActor = nullptr;
		LastSpawnedActor = nullptr;
	}
}

FVector UBuildingManagerComponent::GridPosition(const FVector& InParam) const
{
	const FVector DivideVector =  InParam/GridValue;
	
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * GridValue,UKismetMathLibrary::Round(DivideVector.Y) * GridValue, 40.f );
}

EPivotPosition UBuildingManagerComponent::GetActorPivotPosition(const AActor* Actor)
{
	if (UStaticMeshComponent* StaticMeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>())
	{
		return GetStaticMeshPivotPosition(StaticMeshComponent);
	}

	if (USkeletalMeshComponent* SkeletalMeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>())
	{
		return GetSkeletalMeshPivotPosition(SkeletalMeshComponent);
	}

	return EPivotPosition::Bottom;
}

EPivotPosition UBuildingManagerComponent::GetStaticMeshPivotPosition(const UStaticMeshComponent* MeshComp)
{
	if (MeshComp && MeshComp->GetStaticMesh())
	{
		// 메쉬의 로컬 공간에서 AABB 중심 위치
		const FBox LocalBounds = MeshComp->GetStaticMesh()->GetBoundingBox();
		const FVector Center = LocalBounds.GetCenter();
		const FVector Min = LocalBounds.Min;

		// 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
		float CenterZ = Center.Z;
		float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
		float MinZ = Min.Z;

		// 어느 쪽에 가까운지 판단
		float DistToCenter = FMath::Abs(PivotZ - CenterZ);
		float DistToBottom = FMath::Abs(PivotZ - MinZ);

		if (DistToCenter < DistToBottom)
		{
			return EPivotPosition::Center;
		}
	}
	return EPivotPosition::Bottom;
}

EPivotPosition UBuildingManagerComponent::GetSkeletalMeshPivotPosition(const USkeletalMeshComponent* MeshComp)
{
	if (MeshComp && MeshComp->GetSkeletalMeshAsset())
	{
		// 메쉬의 로컬 공간에서 AABB 중심 위치
		const FBox LocalBounds = MeshComp->GetSkeletalMeshAsset()->GetImportedBounds().GetBox();
		const FVector Center = LocalBounds.GetCenter();
		const FVector Min = LocalBounds.Min;

		// 피벗 위치는 (0,0,0) 이므로 Center.Z 와 Min.Z 비교
		float CenterZ = Center.Z;
		float PivotZ = 0.f; // 피벗은 로컬 공간 기준 (0,0,0)
		float MinZ = Min.Z;

		// 어느 쪽에 가까운지 판단
		float DistToCenter = FMath::Abs(PivotZ - CenterZ);
		float DistToBottom = FMath::Abs(PivotZ - MinZ);

		if (DistToCenter < DistToBottom)
		{
			return EPivotPosition::Center;
		}
	}
	return EPivotPosition::Bottom;
}

bool UBuildingManagerComponent::IsClassInHitList(AActor* Actor)
{
	if (!Actor) return false;

	for (const TSubclassOf<AActor>& ClassType : HitClassTypes)
	{
		if (ClassType && Actor->IsA(ClassType))
		{
			TargetActor = Actor;
			return true;
		}
	}
	return false;
}

bool UBuildingManagerComponent::CheckLineTrace()
{
	FHitResult Result;
	FVector Start = SpawnActor->GetActorLocation() + FVector(0, 0, 1000);
	FVector End = SpawnActor->GetActorLocation() + FVector(0, 0, -2000);
	// DrawDebugLine(
	// 	GetWorld(),
	// 	Start,
	// 	End,
	// 	FColor::Green, // 선 색상
	// 	false, // 영구 표시할지 (false면 일정 시간 후 사라짐)
	// 	5.0f, // 지속 시간 (초)
	// 	0, // 뎁스 프라이어리티
	// 	2.0f // 선 두께
	// );
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SpawnActor);

	if (GetWorld()->LineTraceSingleByChannel(Result, Start, End, CollisionChannel, Params))
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

	if (GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, CollisionChannel, Params))
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



