#include "Animation/CarVehicleAnimInstance.h"

void UCarVehicleAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    /** 4륜은 기울기가 거의 없으므로 기본값을 작게 설정 */
    MaxLeanAngle = 5.f;
}

void UCarVehicleAnimInstance::UpdateWheels(float DeltaSeconds)
{
    ElapsedTime += DeltaSeconds;

    /** 앞바퀴 좌우 회전각 계산 */
    FrontLeftWheelRotation  = CalculateWheelRotation(FrontLeftWheelRotation,  FrontWheelRadius, DeltaSeconds);
    FrontRightWheelRotation = CalculateWheelRotation(FrontRightWheelRotation, FrontWheelRadius, DeltaSeconds);

    /** 뒷바퀴 좌우 회전각 계산 */
    RearLeftWheelRotation   = CalculateWheelRotation(RearLeftWheelRotation,  RearWheelRadius, DeltaSeconds);
    RearRightWheelRotation  = CalculateWheelRotation(RearRightWheelRotation, RearWheelRadius, DeltaSeconds);

    UpdateFrontWheelSteer(DeltaSeconds);
    UpdateBodyBounce(DeltaSeconds);
}

void UCarVehicleAnimInstance::UpdateFrontWheelSteer(float DeltaSeconds)
{
    /** 이동 방향각을 최대 조향각 범위로 클램프해 목표 조향각 산출 */
    const float TargetSteer = FMath::Clamp(MovementDirection, -MaxSteerAngle, MaxSteerAngle);

    /** 부드럽게 보간 */
    FrontWheelSteerAngle = FMath::FInterpTo(FrontWheelSteerAngle, TargetSteer, DeltaSeconds, SteerInterpSpeed);
}

void UCarVehicleAnimInstance::UpdateBodyBounce(float DeltaSeconds)
{
    /** 주행 중일 때만 진동, 속도가 높을수록 진폭 커짐 */
    const float TargetBounce = bIsMoving
        ? FMath::Sin(ElapsedTime * BounceFrequency) * BounceStrength * NormalizedSpeed
        : 0.f;

    /** 멈출 때 진동이 부드럽게 사라지도록 보간 */
    BodyBounceOffset = FMath::FInterpTo(BodyBounceOffset, TargetBounce, DeltaSeconds, 10.f);
}
