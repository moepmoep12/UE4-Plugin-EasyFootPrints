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


void UDefaultRenderTargetComponent::BeginPlay()
{
	
}


bool UDefaultRenderTargetComponent::drawOnRenderTarget(UFootPrintComponent* FPComp)
{
	if (FootPrintComponent != FPComp) {
		FootPrintComponent = FPComp;
	}

	if (hasHitComponentRenderTarget()) {
		createFootPrintOnRenderTarget();
		return true;
	}

	return false;
}

bool UDefaultRenderTargetComponent::hasHitComponentRenderTarget()
{
	UActorComponent* ActorComponent = nullptr;
	ULandscapeComponent* LandscapeComponent = nullptr;
	UFoot* FootOnGround = nullptr;
	UTexture* RenderTexture = nullptr;

	FootOnGround = FootPrintComponent->getFootOnGround();

	if (FootOnGround->getHitresult()->GetActor()) {
		ActorComponent = FootOnGround->getHitresult()->GetActor()->FindComponentByClass(ULandscapeComponent::StaticClass());
		LandscapeComponent = Cast<ULandscapeComponent>(ActorComponent);
	}

	if (!FootOnGround || !LandscapeComponent) {
		return false;
	}

	TArray<UMaterialInterface*> MaterialsOfHitComponent = TArray<UMaterialInterface*>();
	LandscapeComponent->GetUsedMaterials(MaterialsOfHitComponent);

	for (UMaterialInterface* mat : MaterialsOfHitComponent) {
		if (mat->GetTextureParameterValue(FName("RenderTarget"), RenderTexture)) {
			FootOnGround->setHitMaterial(mat);
			if (RenderTexture) {
				RenderTargetOfHitMaterial = Cast<UTextureRenderTarget2D>(RenderTexture);
				return true;
			}
		}
	}

	return false;
}

void UDefaultRenderTargetComponent::createFootPrintOnRenderTarget()
{
	UCanvas* Canvas = nullptr;
	FVector2D ScreenSize = FVector2D(0, 0);
	FDrawToRenderTargetContext Context;
	FVector2D CoordinatePosition = FVector2D(0, 0);
	ACharacter* Player = FootPrintComponent->getPlayer();
	FVector ActorScale = FootPrintComponent->getFootOnGround()->getHitresult()->GetActor()->GetActorScale();

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(Player, RenderTargetOfHitMaterial, Canvas, ScreenSize, Context);

	FVector2D ScreenPosition = InitComputationOfRenderTargetScreenPosition();
	ScreenSize = ComputeRenderTargetScreenSize(ActorScale);
	Canvas->K2_DrawMaterial(FootPrintComponent->M_Spot, ScreenPosition, ScreenSize, CoordinatePosition, FVector2D::UnitVector, FootPrintComponent->getFootOnGround()->getRotation().Yaw);


	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(Player, Context);
	RenderTargetOfHitMaterial = nullptr;

	FootPrintComponent->getFootOnGround()->IncreaseFootPollution();

}

FVector2D UDefaultRenderTargetComponent::InitComputationOfRenderTargetScreenPosition() {
	UFoot* FootOnGround = FootPrintComponent->getFootOnGround();
	FVector ActorLocation = FootOnGround->getHitresult()->GetActor()->GetActorLocation();
	FVector HitLocation = FootOnGround->getHitresult()->Location;
	FVector ActorBounds;
	FVector ActorBoundsOrigin;


	FootOnGround->getHitresult()->GetActor()->GetActorBounds(true, ActorBoundsOrigin, ActorBounds);

	FVector2D ScreenPosition = ComputeScreenPositionOnRenderTarget(ActorLocation, HitLocation, ActorBounds);

	return ScreenPosition;

}

FVector2D UDefaultRenderTargetComponent::ComputeRenderTargetScreenSize(FVector ActorScale) {
	int32 XSize = RenderTargetOfHitMaterial->SizeX;
	int32 YSize = RenderTargetOfHitMaterial->SizeY;

	FVector2D ScreenSize = FVector2D(XSize * (1 / ActorScale.X), YSize * (1 / ActorScale.Y));
	return ScreenSize;
}

FVector2D UDefaultRenderTargetComponent::ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds) {

	int32 XSizeOfRT = RenderTargetOfHitMaterial->SizeX;
	int32 YSizeOfRT = RenderTargetOfHitMaterial->SizeY;

	FVector ActorScale = FootPrintComponent->getFootOnGround()->getHitresult()->GetActor()->GetActorScale();
	FVector2D ActorLocation2D = Get2DVectorWithXAndYFrom3DVector(ActorLocation);
	FVector2D HitLocation2D = Get2DVectorWithXAndYFrom3DVector(HitLocation);
	FVector2D ActorBounds2D = Get2DVectorWithXAndYFrom3DVector(ActorBounds);


	ActorBounds2D *= 2;
	FVector2D ScreenPosition = ActorLocation2D - HitLocation2D;

	float PercentX = UKismetMathLibrary::Abs(ScreenPosition.X / ActorBounds2D.X);
	float PercentY = UKismetMathLibrary::Abs(ScreenPosition.Y / ActorBounds2D.Y);

	ScreenPosition = FVector2D(XSizeOfRT * PercentX, YSizeOfRT * PercentY);
	ScreenPosition = FVector2D(ScreenPosition.X - ScreenPosition.X * (1 / ActorScale.X), ScreenPosition.Y - ScreenPosition.Y * (1 / ActorScale.Y));
	
	return ScreenPosition;
}

FVector2D UDefaultRenderTargetComponent::Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed) {
	return FVector2D(VectorToBeComputed.X, VectorToBeComputed.Y);
}


