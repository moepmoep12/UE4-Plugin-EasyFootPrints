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


void UDefaultRenderTargetComponent::BeginPlay()
{

}




void UDefaultRenderTargetComponent::drawOnRenderTarget(UMaterialInterface * MaterialToDraw, FRenderTargetValues * RenderTargetValues)
{
	this->RenderTargetValues = *RenderTargetValues;

	UCanvas* Canvas = nullptr;
	FVector2D ScreenSize = FVector2D(0, 0);
	FDrawToRenderTargetContext Context;
	FVector2D CoordinatePosition = FVector2D(0, 0);
	FVector2D ScreenPosition;

	if (!RenderTargetValues->RenderTargetOfHitMaterial) {
		return;
	}

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetOwner(), RenderTargetValues->RenderTargetOfHitMaterial, Canvas, ScreenSize, Context);

	ScreenPosition = ComputeScreenPositionOnRenderTarget();
	ScreenSize = ComputeRenderTargetScreenSize();
	/*
	if (RenderTargetValues->Density > 1) {
		UMaterialInstanceDynamic* MatInstance = UMaterialInstanceDynamic::Create(MaterialToDraw, this);
		MatInstance->SetScalarParameterValue(FName("Opacity"), 1 / RenderTargetValues->Density);
		for (int i = 0; i < RenderTargetValues->Density; i++) {
			Canvas->K2_DrawMaterial(MatInstance, ScreenPosition, ScreenSize, CoordinatePosition, FVector2D::UnitVector, RenderTargetValues->Rotation);
		}
	}
	else {
		Canvas->K2_DrawMaterial(MaterialToDraw, ScreenPosition, ScreenSize, CoordinatePosition, FVector2D::UnitVector, RenderTargetValues->Rotation);
	}*/
	Canvas->K2_DrawMaterial(MaterialToDraw, ScreenPosition, ScreenSize, CoordinatePosition, FVector2D::UnitVector, RenderTargetValues->Rotation);
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetOwner(), Context);
}



FVector2D UDefaultRenderTargetComponent::ComputeRenderTargetScreenSize() {
	int32 XSize = RenderTargetValues.RenderTargetOfHitMaterial->SizeX;
	int32 YSize = RenderTargetValues.RenderTargetOfHitMaterial->SizeY;

	FVector2D ScreenSize = FVector2D(XSize * (1 / RenderTargetValues.ActorScale.X), YSize * (1 / RenderTargetValues.ActorScale.Y));
	return ScreenSize;
}

FVector2D UDefaultRenderTargetComponent::ComputeScreenPositionOnRenderTarget() {

	int32 XSizeOfRT = RenderTargetValues.RenderTargetOfHitMaterial->SizeX;
	int32 YSizeOfRT = RenderTargetValues.RenderTargetOfHitMaterial->SizeY;


	FVector2D ScreenPosition = RenderTargetValues.ActorLocation - RenderTargetValues.HitLocation;

	float PercentX = UKismetMathLibrary::Abs(ScreenPosition.X / RenderTargetValues.ActorBounds.X);
	float PercentY = UKismetMathLibrary::Abs(ScreenPosition.Y / RenderTargetValues.ActorBounds.Y);

	ScreenPosition = FVector2D(XSizeOfRT * PercentX, YSizeOfRT * PercentY);
	ScreenPosition = FVector2D(ScreenPosition.X - ScreenPosition.X * (1 / RenderTargetValues.ActorScale.X), ScreenPosition.Y - ScreenPosition.Y * (1 / RenderTargetValues.ActorScale.Y));

	return ScreenPosition;
}


