// Fill out your copyright notice in the Description page of Project Settings.

#include "Default Components/DefaultRenderTargetComponent.h"
#include "LandscapeComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "FootPrintComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

/* Clears all RenderTarget that were used for drawing */
void UDefaultRenderTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UTextureRenderTarget2D* rt : RenderTargets) {
		UKismetRenderingLibrary::ClearRenderTarget2D(GetOwner(), rt);
	}
}




void UDefaultRenderTargetComponent::drawOnRenderTarget_Implementation(UMaterialInterface * FootPrintShape, struct FRenderTargetValues&  RenderTargetValues, FTransform ShapeTransform)
{
	this->RenderTargetValues = RenderTargetValues;
	this->ShapeTransform = ShapeTransform;

	UCanvas* Canvas = nullptr;
	FVector2D ScreenSize = FVector2D(0, 0);
	FDrawToRenderTargetContext Context;
	FVector2D CoordinatePosition = FVector2D(0, 0);
	FVector2D ScreenPosition;

	if (!RenderTargetValues.RenderTargetOfHitMaterial) {
		return;
	}

	if (!RenderTargets.Contains(RenderTargetValues.RenderTargetOfHitMaterial)) {
		RenderTargets.Add(RenderTargetValues.RenderTargetOfHitMaterial);
	}

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetOwner(), RenderTargetValues.RenderTargetOfHitMaterial, Canvas, ScreenSize, Context);

	ScreenPosition = CalculateScreenPosition();
	ScreenSize = CalculateFootShapeSize();

	Canvas->K2_DrawMaterial(FootPrintShape, ScreenPosition, ScreenSize, CoordinatePosition, FVector2D::UnitVector, RenderTargetValues.Rotation);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetOwner(), Context);
}



FVector2D UDefaultRenderTargetComponent::CalculateFootShapeSize() {
	int32 XSize = RenderTargetValues.RenderTargetOfHitMaterial->SizeX;
	int32 YSize = RenderTargetValues.RenderTargetOfHitMaterial->SizeY;

	FVector2D ScreenSize = FVector2D(ShapeTransform.GetLocation().X/0.577f * (XSize / RenderTargetValues.ActorBounds.X), ShapeTransform.GetLocation().Y / 0.577f * (YSize / RenderTargetValues.ActorBounds.Y));
	ScreenSize = FVector2D(ScreenSize.X * ShapeTransform.GetScale3D().X, ScreenSize.Y * ShapeTransform.GetScale3D().Y);
	return ScreenSize;
}

FVector2D UDefaultRenderTargetComponent::CalculateScreenPosition() {
	FVector2D ScreenPosition(0, 0);
	int32 RenderTargetSizeX = RenderTargetValues.RenderTargetOfHitMaterial->SizeX;
	int32 RenderTargetSizeY = RenderTargetValues.RenderTargetOfHitMaterial->SizeY;
	FVector2D FootPrintShapeSize = CalculateFootShapeSize();


	// calculate a relative position on the landscape 
	FVector2D RelativePosition = RenderTargetValues.ActorLocation - RenderTargetValues.HitLocation;

	// calculates the offset that needs to be adjusted due to scaling of the landscape
	FVector2D offset = CalculatePositionOffset(RelativePosition);

	// transform the relative position adjusted by the offset into a percentage of the overall landscape size 
	float PercentX = UKismetMathLibrary::Abs((RelativePosition.X - offset.X) / RenderTargetValues.ActorBounds.X);
	float PercentY = UKismetMathLibrary::Abs((RelativePosition.Y - offset.Y) / RenderTargetValues.ActorBounds.Y);


	ScreenPosition = FVector2D(RenderTargetSizeX * PercentX, RenderTargetSizeY * PercentY);

	// move the FootPrintShape into the centre of its coordinate system
	ScreenPosition = FVector2D(ScreenPosition.X - (0.5f * FootPrintShapeSize.X), ScreenPosition.Y - (0.5f * FootPrintShapeSize.Y));

	return ScreenPosition;
}

FVector2D UDefaultRenderTargetComponent::CalculatePositionOffset(FVector2D Position) {
	FVector2D offset(0, 0);

	// Value between 0 - 1 , where 0 = no offset  and 1 = maximum offset
	float distanceFactorXFromCentre = ((RenderTargetValues.ActorBounds.X * 0.5f) - UKismetMathLibrary::Abs(Position.X)) / (RenderTargetValues.ActorBounds.X * 0.5f);
	float distanceFactorYFromCentre = ((RenderTargetValues.ActorBounds.Y * 0.5f) - UKismetMathLibrary::Abs(Position.Y)) / (RenderTargetValues.ActorBounds.Y * 0.5f);

	// calculate offset 
	float offsetX = distanceFactorXFromCentre * 0.5f * RenderTargetValues.ActorScale.X;
	float offsetY = distanceFactorYFromCentre * 0.5f * RenderTargetValues.ActorScale.Y;

	offset = FVector2D(offsetX, offsetY);

	return offset;
}


