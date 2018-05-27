// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMovementAdjustmentComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FootPrintComponent.h"

void UDefaultMovementAdjustmentComp::initComponent(UMovementComponent* MovementComponent)
{
	CharMovementComp = Cast<UCharacterMovementComponent>(MovementComponent);
	saveOriginalMovementValues();
}


void UDefaultMovementAdjustmentComp::saveOriginalMovementValues()
{
	if (!CharMovementComp) {
		return;
	}
	OriginaMaxWalkingSpeed = CharMovementComp->MaxWalkSpeed;
	OriginalJumpVelocity = CharMovementComp->JumpZVelocity;
}


void UDefaultMovementAdjustmentComp::adjustMovement_Implementation(float depth)
{
	if (!CharMovementComp) {
		return;
	}
	adjustMaxWalkSpeed(depth);
	adjustJumpVelocity(depth);
}

void UDefaultMovementAdjustmentComp::adjustMaxWalkSpeed(float depth)
{
	float adjustedWalkSpeed = OriginaMaxWalkingSpeed * (1 / depth);

	if (adjustedWalkSpeed != CurrentMaxWalkSpeed) {
		CharMovementComp->MaxWalkSpeed = adjustedWalkSpeed;
		CurrentMaxWalkSpeed = adjustedWalkSpeed;
	}
}

void UDefaultMovementAdjustmentComp::adjustJumpVelocity(float depth)
{
	float adjustedJumpVelocity = OriginalJumpVelocity - OriginalJumpVelocity * (1 / depth);

	if (adjustedJumpVelocity != CurrentJumpVelocity) {
		CharMovementComp->JumpZVelocity = adjustedJumpVelocity;
		CurrentJumpVelocity = adjustedJumpVelocity;
	}
}


void UDefaultMovementAdjustmentComp::resetMovement_Implementation()
{
	if (!CharMovementComp) {
		return;
	}

	if (CharMovementComp->MaxWalkSpeed != OriginaMaxWalkingSpeed) {
		CharMovementComp->MaxWalkSpeed = OriginaMaxWalkingSpeed;
	}
	CurrentMaxWalkSpeed = OriginaMaxWalkingSpeed;

	if (CharMovementComp->JumpZVelocity != CurrentJumpVelocity) {
		CharMovementComp->JumpZVelocity = OriginalJumpVelocity;
	}
	CurrentJumpVelocity = OriginalJumpVelocity;
}



