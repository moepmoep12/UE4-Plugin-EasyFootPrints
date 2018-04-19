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
	UPROPERTY()
		FTransform ShapeTransform;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Calculates the size of the footprint shape that is drawn onto the rendertarget
	*	will be adjusted by scale set in ShapeTransform
	*/
	FVector2D CalculateFootShapeSize();
	FVector2D CalculateScreenPosition();
	FVector2D CalculatePositionOffset(FVector2D Position);
	
public:
	 void drawOnRenderTarget_Implementation(UMaterialInterface * FootPrintShape,struct FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform) override;

};
