// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BaseRenderTargetComponent.h"
#include "DefaultRenderTargetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UDefaultRenderTargetComponent : public UBaseRenderTargetComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UFootPrintComponent* FootPrintComponent;
	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool hasHitComponentRenderTarget();
	void createFootPrintOnRenderTarget();
	FVector2D InitComputationOfRenderTargetScreenPosition();
	FVector2D ComputeRenderTargetScreenSize(FVector ActorScale);
	FVector2D ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds);
	FVector2D Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed);
	
public:
	 bool drawOnRenderTarget(UFootPrintComponent* FPComp) override;

};
