// Fill out your copyright notice in the Description page of Project Settings.
#include "Animation/VehicleAnimInstance.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UVehicleAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwnerPawn = TryGetPawnOwner();
    if (OwnerPawn)
    {
        MovementComponent = OwnerPawn->GetMovementComponent();
    }
}

void UVehicleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerPawn) return;

    UpdateSpeed();
    UpdateDirection(DeltaSeconds);
    UpdateState();
    UpdateLean(DeltaSeconds);
}

void UVehicleAnimInstance::UpdateSpeed()
{
    if (!MovementComponent) return;

    const FVector Velocity = MovementComponent->Velocity;

    /** cm/s 단위 속도 */
    Speed = Velocity.Size();

    /** cm/s → km/h 변환 */
    SpeedKmH = Speed * 0.036f;

    /** MaxSpeed 기준 0~1 정규화 */
    NormalizedSpeed = MaxSpeed > 0.f ? FMath::Clamp(Speed / MaxSpeed, 0.f, 1.f) : 0.f;

    /** 입력 방향 * 속도로 가속도 근사 */
    Acceleration = MovementComponent->GetLastInputVector() * Speed;

    /** 입력값이 있으면 가속 중으로 판단 */
    bIsAccelerating = !MovementComponent->GetLastInputVector().IsNearlyZero();
}

void UVehicleAnimInstance::UpdateDirection(float DeltaSeconds)
{
    if (!OwnerPawn || !MovementComponent) return;

    const FVector Velocity = MovementComponent->Velocity;

    if (Velocity.SizeSquared() > KINDA_SMALL_NUMBER)
    {
        const FRotator ActorRot    = OwnerPawn->GetActorRotation();
        const FRotator VelocityRot = Velocity.Rotation();

        /** 액터 전방과 속도 방향 사이의 Yaw 차이 (-180 ~ 180) */
        MovementDirection = UKismetMathLibrary::NormalizedDeltaRotator(VelocityRot, ActorRot).Yaw;
    }
    else
    {
        MovementDirection = 0.f;
    }

    /** 방향각 ±90도 이내면 전진 */
    bIsMovingForward  = MovementDirection > -90.f && MovementDirection < 90.f;

    /** 전진이 아니고 속도가 임계값 이상이면 후진 */
    bIsMovingBackward = !bIsMovingForward && Speed > MovingSpeedThreshold;
}

void UVehicleAnimInstance::UpdateState()
{
    if (!MovementComponent) return;

    /** 속도가 임계값을 넘으면 이동 중으로 판단 */
    bIsMoving = Speed > MovingSpeedThreshold;

    /** 낙하 중이면 공중 상태 */
    bIsInAir = MovementComponent->IsFalling();

    /** 컨트롤러가 존재하면 탑승 중으로 판단 */
    bIsOccupied = OwnerPawn->GetController() != nullptr;
}

void UVehicleAnimInstance::UpdateLean(const float DeltaSeconds)
{
    /** 이동 방향을 -1~1로 정규화한 뒤 최대 각도를 곱해 목표 기울기 산출 */
    const float TargetLean = FMath::Clamp(MovementDirection / 90.f, -1.f, 1.f) * MaxLeanAngle;

    /** 현재 기울기에서 목표 기울기로 부드럽게 보간 */
    LeanAngle = FMath::FInterpTo(LeanAngle, TargetLean, DeltaSeconds, LeanInterpSpeed);
}
