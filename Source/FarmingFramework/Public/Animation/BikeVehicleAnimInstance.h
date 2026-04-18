#pragma once

#include "CoreMinimal.h"
#include "VehicleAnimInstance.h"
#include "BikeVehicleAnimInstance.generated.h"

UCLASS()
class FARMINGFRAMEWORK_API UBikeVehicleAnimInstance : public UVehicleAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;

protected:
    /** 앞바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Bike|Wheel")
    float FrontWheelRotation;

    /** 뒷바퀴 회전각 */
    UPROPERTY(BlueprintReadOnly, Category = "Bike|Wheel")
    float RearWheelRotation;

    /** 앞바퀴 반지름 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bike|Wheel")
    float FrontWheelRadius = 30.f;

    /** 뒷바퀴 반지름 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bike|Wheel")
    float RearWheelRadius = 30.f;

    /** 핸들 조향각 (Z축) */
    UPROPERTY(BlueprintReadOnly, Category = "Bike|Handle")
    float HandleSteerAngle;

    /** 핸들 최대 조향각 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bike|Handle")
    float MaxHandleSteerAngle = 45.f;

    /** 핸들 보간 속도 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bike|Handle")
    float HandleInterpSpeed = 6.f;

    virtual void UpdateWheels(float DeltaSeconds) override;

private:
    void UpdateHandleSteer(float DeltaSeconds);
};
