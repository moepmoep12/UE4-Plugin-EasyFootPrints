// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseMovementAdjustmentComponent.generated.h"

// Forward Declaration
class UFootPrintComponent;

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
	virtual void adjustMovement() {};
	virtual void resetMovement() {};
	virtual void initComponent(UFootPrintComponent* FPComp) {};
	
};
