// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseMovementAdjustmentComponent.generated.h"

// Forward Declaration
class UFootPrintComponent;


/** This is an abstract base class for a MovementAdjustment Component.
*   When a player walks through a material with tessellation the component is responsible for adjusting the movement
*/
UCLASS( ClassGroup=(EasyFootPrints), abstract )
class EASYFOOTPRINTS_API UBaseMovementAdjustmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	//UBaseMovementAdjustmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};

	/* adjusts the character movement based on the depth of the material*/
	virtual void adjustMovement(float depth) {};

	/* restores the values of the movement component to its original*/
	virtual void resetMovement() {};

	/** Should be used to create a pointer to the character movement component that will be changed by this component and also save the orignal values*/
	virtual void initComponent(UCharacterMovementComponent* MovementComponent) {};
	
};
