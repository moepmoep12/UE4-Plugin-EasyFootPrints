// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMovementAdjustmentComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDefaultMovementAdjustmentComp::initComponent(UFootPrintComponent * FPComp)
{
		FootPrintComponent = FPComp;
		saveOriginalMovementValues();
}


void UDefaultMovementAdjustmentComp::saveOriginalMovementValues()
{
	MovementComponent = Cast<UCharacterMovementComponent>(FootPrintComponent->getPlayer()->GetMovementComponent());
	OriginaMaxWalkingSpeed = MovementComponent->MaxWalkSpeed;
	OriginalJumpVelocity = MovementComponent->JumpZVelocity;
}


void UDefaultMovementAdjustmentComp::adjustMovement()
{
	float depth = FootPrintComponent->getFootOnGround()->getDepth();
	adjustMaxWalkSpeed(depth);
	adjustJumpVelocity(depth);
}

void UDefaultMovementAdjustmentComp::adjustMaxWalkSpeed(float depth)
{
	float adjustedWalkSpeed = OriginaMaxWalkingSpeed * (1 / depth);

	if (adjustedWalkSpeed != CurrentMaxWalkSpeed) {
		Cast<UCharacterMovementComponent>(FootPrintComponent->getPlayer()->GetMovementComponent())->MaxWalkSpeed = adjustedWalkSpeed;
		CurrentMaxWalkSpeed = adjustedWalkSpeed;
	}
}

void UDefaultMovementAdjustmentComp::adjustJumpVelocity(float depth)
{
	float adjustedJumpVelocity = OriginalJumpVelocity - OriginalJumpVelocity * (1 / depth);

	if (adjustedJumpVelocity != CurrentJumpVelocity) {
		Cast<UCharacterMovementComponent>(FootPrintComponent->getPlayer()->GetMovementComponent())->JumpZVelocity = adjustedJumpVelocity;
		CurrentJumpVelocity = adjustedJumpVelocity;
	}
}


void UDefaultMovementAdjustmentComp::resetMovement()
{
	if (MovementComponent->MaxWalkSpeed != OriginaMaxWalkingSpeed) {
		MovementComponent->MaxWalkSpeed = OriginaMaxWalkingSpeed;
	}
	CurrentMaxWalkSpeed = OriginaMaxWalkingSpeed;

	if (MovementComponent->JumpZVelocity != CurrentJumpVelocity) {
		MovementComponent->JumpZVelocity = OriginalJumpVelocity;
	}
	CurrentJumpVelocity = OriginalJumpVelocity;
}



