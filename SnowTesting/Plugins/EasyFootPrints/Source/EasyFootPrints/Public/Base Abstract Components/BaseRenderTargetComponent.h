// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseRenderTargetComponent.generated.h"

class UFootPrintComponent;

UCLASS( ClassGroup=(EasyFootPrints), abstract )
class EASYFOOTPRINTS_API UBaseRenderTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	//UBaseRenderTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};
	virtual bool drawOnRenderTarget(UFootPrintComponent* FPComp) { return false; };
	
		
	
};
