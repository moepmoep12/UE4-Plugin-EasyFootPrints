// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMovementAdjustmentComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FootPrintComponent.h"

void UDefaultMovementAdjustmentComp::initComponent(UCharacterMovementComponent* MovementComponent)
{
	this->MovementComponent = MovementComponent;
	saveOriginalMovementValues();
}


void UDefaultMovementAdjustmentComp::saveOriginalMovementValues()
{
	//MovementComponent = Cast<UCharacterMovementComponent>(FootPrintComponent->getPlayer()->GetMovementComponent());
	OriginaMaxWalkingSpeed = MovementComponent->MaxWalkSpeed;
	OriginalJumpVelocity = MovementComponent->JumpZVelocity;
}


void UDefaultMovementAdjustmentComp::adjustMovement_Implementation(float depth)
{
	adjustMaxWalkSpeed(depth);
	adjustJumpVelocity(depth);
}

void UDefaultMovementAdjustmentComp::adjustMaxWalkSpeed(float depth)
{
	float adjustedWalkSpeed = OriginaMaxWalkingSpeed * (1 / depth);

	if (adjustedWalkSpeed != CurrentMaxWalkSpeed) {
		MovementComponent->MaxWalkSpeed = adjustedWalkSpeed;
		CurrentMaxWalkSpeed = adjustedWalkSpeed;
	}
}

void UDefaultMovementAdjustmentComp::adjustJumpVelocity(float depth)
{
	float adjustedJumpVelocity = OriginalJumpVelocity - OriginalJumpVelocity * (1 / depth);

	if (adjustedJumpVelocity != CurrentJumpVelocity) {
		MovementComponent->JumpZVelocity = adjustedJumpVelocity;
		CurrentJumpVelocity = adjustedJumpVelocity;
	}
}


void UDefaultMovementAdjustmentComp::resetMovement_Implementation()
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



