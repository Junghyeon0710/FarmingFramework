// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Building/BuildingManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UBuildingManagerComponent::UBuildingManagerComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}

void UBuildingManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UBuildingManagerComponent::BuildStart(const TSubclassOf<AActor>& TargetClass)
{
	check(TargetClass);
	SpawnActor = GetWorld()->SpawnActor(TargetClass);
	bIsSpawn = true;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PC->SetShowMouseCursor(true);
	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	PC->DeprojectMousePositionToWorld(MouseWorldLocation,MouseWorldDirection);

	
	
}

FVector UBuildingManagerComponent::GridPosition(const FVector& InParam,int32 GridValue) const
{
	const FVector DivideVector =  InParam/GridValue;
	
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * GridValue,UKismetMathLibrary::Round(DivideVector.Y) * GridValue, 40.f );
}

void UBuildingManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bIsSpawn)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FVector MouseWorldLocation;
		FVector MouseWorldDirection;

		if (PC && PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
		{
			FVector TraceStart = MouseWorldLocation;
			FVector TraceEnd = MouseWorldLocation + (MouseWorldDirection * 100000.f);

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.bTraceComplex = true;
			Params.AddIgnoredActor(GetOwner()); // 자기 자신 무시 (필요시)
			Params.AddIgnoredActor(SpawnActor);
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				Params
			);

			if (bHit)
			{
				// 마우스가 가리키는 실제 지점
				FVector HitLocation = HitResult.Location;

				// 100cm (1m) 간격으로 스냅
				FVector SnappedLocation = HitLocation.GridSnap(100.f);
				SnappedLocation.Z = HitLocation.Z;
				// 액터 위치 설정
				if (SpawnActor)
				{
					SpawnActor->SetActorLocation(SnappedLocation);
				}

				// 디버그 메시지
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Hit: %s"), *HitLocation.ToString()));
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Snapped: %s"), *SnappedLocation.ToString()));
			}
		}
	}
}

