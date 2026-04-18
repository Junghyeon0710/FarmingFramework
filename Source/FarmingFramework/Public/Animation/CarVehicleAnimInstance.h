#pragma once

#include "CoreMinimal.h"
#include "VehicleAnimInstance.h"
#include "CarVehicleAnimInstance.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API UCarVehicleAnimInstance : public UVehicleAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;

protected:
    /** 앞 왼쪽 바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Wheel")
    float FrontLeftWheelRotation;

    /** 앞 오른쪽 바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Wheel")
    float FrontRightWheelRotation;

    /** 뒤 왼쪽 바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Wheel")
    float RearLeftWheelRotation;

    /** 뒤 오른쪽 바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Wheel")
    float RearRightWheelRotation;

    /** 앞바퀴 반지름 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Wheel")
    float FrontWheelRadius = 50.f;

    /** 뒷바퀴 반지름 (앞뒤 크기가 다를 수 있음) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Wheel")
    float RearWheelRadius = 50.f;

    /** 앞바퀴 조향각 (Z축) */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Steer")
    float FrontWheelSteerAngle;

    /** 앞바퀴 최대 조향각 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Steer")
    float MaxSteerAngle = 35.f;

    /** 조향 보간 속도 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Steer")
    float SteerInterpSpeed = 5.f;

    /** 차체 상하 진동 오프셋 */
    UPROPERTY(BlueprintReadOnly, Category = "Car|Body")
    float BodyBounceOffset;

    /** 진동 세기 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Body")
    float BounceStrength = 2.f;

    /** 진동 주기 속도 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Car|Body")
    float BounceFrequency = 8.f;

    virtual void UpdateWheels(float DeltaSeconds) override;

private:
    void UpdateFrontWheelSteer(float DeltaSeconds);
    void UpdateBodyBounce(float DeltaSeconds);

    /** Sin파 계산용 경과 시간 누적 */
    float ElapsedTime = 0.f;
};
