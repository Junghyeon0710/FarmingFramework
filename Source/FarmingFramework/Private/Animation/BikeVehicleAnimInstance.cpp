#include "Animation/BikeVehicleAnimInstance.h"

void UBikeVehicleAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    /** 2륜은 기울기 표현이 중요하므로 기본값을 크게 설정 */
    MaxLeanAngle = 25.f;
}

// void UBikeVehicleAnimInstance::UpdateWheels(float DeltaSeconds)
// {
//     /** 앞바퀴 반지름 기준으로 회전각 계산 */
//     FrontWheelRotation = CalculateWheelRotation(FrontWheelRotation, FrontWheelRadius, DeltaSeconds);
//
//     /** 뒷바퀴 반지름 기준으로 회전각 계산 */
//     RearWheelRotation = CalculateWheelRotation(RearWheelRotation, RearWheelRadius, DeltaSeconds);
//
//     UpdateHandleSteer(DeltaSeconds);
// }

void UBikeVehicleAnimInstance::UpdateHandleSteer(float DeltaSeconds)
{
    /** 이동 방향 기준 목표 조향각 산출 */
    const float TargetSteer = FMath::Clamp(MovementDirection, -MaxHandleSteerAngle, MaxHandleSteerAngle);

    /** 부드럽게 보간 */
    HandleSteerAngle = FMath::FInterpTo(HandleSteerAngle, TargetSteer, DeltaSeconds, HandleInterpSpeed);
}
