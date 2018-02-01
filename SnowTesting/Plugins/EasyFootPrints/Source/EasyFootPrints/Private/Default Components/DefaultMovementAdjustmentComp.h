// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BaseMovementAdjustmentComponent.h"
#include "DefaultMovementAdjustmentComp.generated.h"

class UCharacterMovementComponent;

UCLASS()
class UDefaultMovementAdjustmentComp : public UBaseMovementAdjustmentComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
		float OriginaMaxWalkingSpeed;

	UPROPERTY()
		float CurrentMaxWalkSpeed;

	UPROPERTY()
		float OriginalJumpVelocity;

	UPROPERTY()
		float CurrentJumpVelocity;

	UPROPERTY()
		UCharacterMovementComponent* MovementComponent;

protected:
	void saveOriginalMovementValues();
	void adjustMaxWalkSpeed(float depth);
	void adjustJumpVelocity(float depth);


public:
	virtual void initComponent(UCharacterMovementComponent* MovementComponent) override;
	virtual void adjustMovement(float depth) override;
	virtual void resetMovement() override;
	
};
