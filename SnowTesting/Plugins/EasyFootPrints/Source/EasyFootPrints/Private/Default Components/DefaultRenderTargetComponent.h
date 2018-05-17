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
	// a struct of values needed for drawing onto render target
	UPROPERTY()
		FRenderTargetValues RenderTargetValues;

	// The transform of the shape material that is used to draw onto the rendertarget
	UPROPERTY()
		FTransform ShapeTransform;

	// An array of all RenderTargets; used for clearing them when Play ends 
	UPROPERTY()
		TArray<UTextureRenderTarget2D*> RenderTargets;

protected:
	virtual void BeginPlay() override {	Super::BeginPlay(); };
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Calculates the size of the footprint shape that is drawn onto the rendertarget
	*	will be adjusted by scale set in ShapeTransform
	*/
	FVector2D CalculateFootShapeSize();

	/* Calculates the screen position of the footprint shape on the rendertarget */
	FVector2D CalculateScreenPosition();

	/* Calculates the offset of the screenposition that occurs due to scaling of the landscape */
	FVector2D CalculatePositionOffset(FVector2D Position);

public:
	void drawOnRenderTarget_Implementation(UMaterialInterface * FootPrintShape, const FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform) override;

};
