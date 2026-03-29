// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VehicleAnimInstance.generated.h"

class APawn;
class UPawnMovementComponent;

UCLASS()
class FARMINGFRAMEWORK_API UVehicleAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    /** 소유 폰 참조 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Reference")
    TObjectPtr<APawn> OwnerPawn;

    /** 이동 컴포넌트 참조 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Reference")
    TObjectPtr<UPawnMovementComponent> MovementComponent;

    /** 현재 이동 속도 (cm/s) */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Speed")
    float Speed;

    /** 현재 이동 속도 (km/h 환산) */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Speed")
    float SpeedKmH;

    /** 속도 정규화 값 (0.0 ~ 1.0), MaxSpeed 기준 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Speed")
    float NormalizedSpeed;

    /** 최대 속도 (cm/s) — 블루프린트에서 차량마다 설정 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Speed")
    float MaxSpeed = 1200.f;

    /** 이동 방향각 (Yaw 기준, -180 ~ 180) */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Direction")
    float MovementDirection;

    /** 전진 중인지 여부 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Direction")
    bool bIsMovingForward;

    /** 후진 중인지 여부 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Direction")
    bool bIsMovingBackward;

    /** 움직이고 있는지 여부 (Speed > Threshold) */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|State")
    bool bIsMoving;

    /** 공중에 떠 있는지 여부 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|State")
    bool bIsInAir;

    /** 탑승 중인지 여부 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|State")
    bool bIsOccupied;

    /** 현재 가속도 벡터 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Acceleration")
    FVector Acceleration;

    /** 가속 중인지 여부 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Acceleration")
    bool bIsAccelerating;

    /** 좌우 기울기 (Roll), 회전 시 바디 틸팅에 활용 */
    UPROPERTY(BlueprintReadOnly, Category = "Vehicle|Lean")
    float LeanAngle;

    /** 기울기 보간 속도 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Lean")
    float LeanInterpSpeed = 8.f;

    /** 최대 기울기 각도 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Lean")
    float MaxLeanAngle = 15.f;

    /** 이 속도 이상이면 bIsMoving = true */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Speed")
    float MovingSpeedThreshold = 10.f;

private:
    void UpdateSpeed();
    void UpdateDirection(float DeltaSeconds);
    void UpdateState();
    void UpdateLean(float DeltaSeconds);
};
