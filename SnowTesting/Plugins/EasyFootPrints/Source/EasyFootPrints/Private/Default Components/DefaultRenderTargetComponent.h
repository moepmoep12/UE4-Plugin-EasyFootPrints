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
		FRenderTargetValues RenderTargetValues;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FVector2D ComputeRenderTargetScreenSize();
	FVector2D ComputeScreenPositionOnRenderTarget();
	
public:
	 void drawOnRenderTarget(UMaterialInterface* MaterialToDraw, FRenderTargetValues* RenderTargetValues) override;

};
