// Fill out your copyright notice in the Description page of Project Settings.

#include "RenderTargetComputations.h"
#include "FootPrintComponent.h"
#include "Foot.h"
#include "LandscapeComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values for this component's properties
URenderTargetComputations::URenderTargetComputations()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void URenderTargetComputations::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void URenderTargetComputations::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URenderTargetComputations::computeRenderTarget()
{
	if (hasHitComponentRenderTarget()) {
		createFootPrintOnRenderTarget();
		return true;
	}

	return false;
}

/*
*	Checks whether the hit component has a MaterialInstance with a RenderTarget
*	if true set RenderTargetOfHitMaterial
*/
bool URenderTargetComputations::hasHitComponentRenderTarget()
{
	UActorComponent* ActorComponent = nullptr;
	ULandscapeComponent* LandscapeComponent = nullptr;
	UFoot* FootOnGround = nullptr;
	UTexture* RenderTexture;
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

void URenderTargetComputations::createFootPrintOnRenderTarget()
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
	Canvas->K2_DrawMaterial(FootPrintComponent->M_Spot, ScreenPosition, ScreenSize, CoordinatePosition,FVector2D::UnitVector, FootPrintComponent->getFootOnGround()->getRotation().Yaw );


	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(Player, Context);
	RenderTargetOfHitMaterial = nullptr;

	FootPrintComponent->getFootOnGround()->IncreaseFootPollution();

}

FVector2D URenderTargetComputations::InitComputationOfRenderTargetScreenPosition() {
	UFoot* FootOnGround = FootPrintComponent->getFootOnGround();
	FVector ActorLocation = FootOnGround->getHitresult()->GetActor()->GetActorLocation();
	FVector HitLocation = FootOnGround->getHitresult()->Location;
	FVector ActorBounds;
	FVector ActorBoundsOrigin;
	

	FootOnGround->getHitresult()->GetActor()->GetActorBounds(true, ActorBoundsOrigin, ActorBounds);

	FVector2D ScreenPosition = ComputeScreenPositionOnRenderTarget(ActorLocation, HitLocation, ActorBounds);

	return ScreenPosition;

}

FVector2D URenderTargetComputations::ComputeRenderTargetScreenSize(FVector ActorScale) {
	int32 XSize = RenderTargetOfHitMaterial->SizeX;
	int32 YSize = RenderTargetOfHitMaterial->SizeY;

	FVector2D ScreenSize = FVector2D(XSize * (1/ ActorScale.X), YSize * (1/ActorScale.Y));
	return ScreenSize;
}

FVector2D URenderTargetComputations::ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds) {

	int32 XSizeOfRT = RenderTargetOfHitMaterial->SizeX;
	int32 YSizeOfRT = RenderTargetOfHitMaterial->SizeY;
	//FVector2D RenderTargetSize = FVector2D(XSizeOfRT, YSizeOfRT);
	FVector ActorScale = FootPrintComponent->getFootOnGround()->getHitresult()->GetActor()->GetActorScale();
	FVector2D ActorLocation2D = Get2DVectorWithXAndYFrom3DVector(ActorLocation);
	FVector2D HitLocation2D = Get2DVectorWithXAndYFrom3DVector(HitLocation);
	FVector2D ActorBounds2D = Get2DVectorWithXAndYFrom3DVector(ActorBounds);


	ActorBounds2D *= 2;
	FVector2D ScreenPosition = ActorLocation2D - HitLocation2D;
	float PercentX = UKismetMathLibrary::Abs(ScreenPosition.X / ActorBounds2D.X);
	float PercentY = UKismetMathLibrary::Abs(ScreenPosition.Y / ActorBounds2D.Y);

	//FVector2D ScreenPositionPart1 = FVector2D(XSizeOfRT * (ScreenPosition.X - 0.5), YSizeOfRT * (ScreenPosition.Y - 0.5));
	//FVector2D ScreenPositionPart2 = FVector2D(XSizeOfRT * 0.5 * (1-(1/ActorScale.X)), YSizeOfRT * 0.5 * (1-(1/ActorScale.Y)));
	//ScreenPosition = ScreenPositionPart1 + ScreenPositionPart2;
	//new calculation
	ScreenPosition = FVector2D(XSizeOfRT * PercentX , YSizeOfRT * PercentY);
	ScreenPosition = FVector2D(ScreenPosition.X - ScreenPosition.X * (1 / ActorScale.X), ScreenPosition.Y - ScreenPosition.Y * (1 / ActorScale.Y));
	//ScreenPosition = RenderTargetSize - ScreenPosition;
	return ScreenPosition;
}

FVector2D URenderTargetComputations::Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed) {
	return FVector2D(VectorToBeComputed.X, VectorToBeComputed.Y);
}