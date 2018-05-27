// Fill out your copyright notice in the Description page of Project Settings.

#include "Foot.h"
#include "Engine/GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "LandscapeComponent.h"
#include "Engine/EngineTypes.h"
#include "Materials/MaterialInterface.h"
#include "PhysMaterial_EasyFootPrints.h"
#include "Base Abstract Components/BasePollutionComponent.h"
#include "Default Components/DefaultPollutionComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


void UFoot::setRotation(FVector ForwardVector)
{
	FQuat floorRot = FRotationMatrix::MakeFromZX(Hitresult.Normal, ForwardVector).ToQuat();
	FQuat offsetRot(FRotator(0.0f, 90.0f, 0.0f));
	Rotation = (floorRot * offsetRot).Rotator();
}

void UFoot::initPollutionComponent(TSubclassOf<UBasePollutionComponent> PollutionComp)
{
	PollutionComponent = NewObject<UBasePollutionComponent>(this, PollutionComp);
}

void UFoot::IncreaseFootPollution()
{
	if (!PhysMat) {
		return; 
	}
	PollutionComponent->increasePollution(PhysMat);
}

void UFoot::createPollutionFootPrint(UWorld* world, FVector Scale)
{
	FTransform transform = FTransform(Rotation, Location, Scale);
	PollutionComponent->createPollutionFootPrint(transform, PollutionMaterial, world);
}

float UFoot::getDepth()
{
	float depth = 0;
	HitMaterial->GetScalarParameterValue(FName("Depth"), depth);
	return depth;
}

float UFoot::getTessellationHeight() {

	float tessellationHeight = 0;
	if (HitMaterial) {
		return HitMaterial->GetScalarParameterValue(FName("TessellationHeight"), tessellationHeight) ? tessellationHeight : 0.0f;
	}
	return 0.0f;
}

bool UFoot::hasHitComponentRenderTarget()
{
	return RenderTargetValues.RenderTargetOfHitMaterial != nullptr;
}

void UFoot::updateHitMaterial()
{
	if (Hitresult.Actor == nullptr) {
		return;
	}

	UActorComponent* ActorComponent = nullptr;
	ULandscapeComponent* LandscapeComponent = nullptr;
	UTexture* RenderTexture = nullptr;
	UMaterialInterface* MaterialOfHitComponent = nullptr;

	ActorComponent = Hitresult.Actor->FindComponentByClass(ULandscapeComponent::StaticClass());
	LandscapeComponent = Cast<ULandscapeComponent>(ActorComponent);

	if (LandscapeComponent) {
		MaterialOfHitComponent = LandscapeComponent->GetLandscapeMaterial();
		MaterialOfHitComponent->GetTextureParameterValue(FName("RenderTarget"), RenderTexture);
	}

	HitMaterial = MaterialOfHitComponent;
	if (HitMaterial) {
		PhysMat = Cast<UPhysMaterial_EasyFootPrints>(HitMaterial->GetPhysicalMaterial());
	}
	RenderTargetValues.RenderTargetOfHitMaterial = Cast<UTextureRenderTarget2D>(RenderTexture);

}

FRenderTargetValues * UFoot::getRenderTargetValues()
{
	updateRenderTargetValues();
	return &RenderTargetValues;
}

void UFoot::updateRenderTargetValues()
{
	if (Hitresult.Actor == nullptr) {
		RenderTargetValues.clear();
		return;
	}
	FVector ActorBounds;
	FVector ActorOrigins;
	Hitresult.Actor->GetActorBounds(true, ActorOrigins, ActorBounds);
	RenderTargetValues.ActorBounds = FVector2D(ActorBounds.X * 2, ActorBounds.Y * 2);
	RenderTargetValues.ActorLocation = FVector2D(Hitresult.Actor->GetActorLocation().X, Hitresult.Actor->GetActorLocation().Y);
	RenderTargetValues.ActorScale = FVector2D(Hitresult.Actor->GetActorScale().X, Hitresult.Actor->GetActorScale().Y);
	RenderTargetValues.HitLocation = FVector2D(Hitresult.Location.X, Hitresult.Location.Y);
	RenderTargetValues.Rotation = Rotation.Yaw;
	RenderTargetValues.Density = (PhysMat != nullptr) ? PhysMat->Density : 0;
}

UParticleSystem * UFoot::getParticleEffect()
{

	return PhysMat != nullptr ? PhysMat->ParticleSystem : nullptr;
}

UParticleSystem * UFoot::getPollutionParticleEffect()
{
	return PhysMat != nullptr ? PhysMat->PollitionParticleSystem : nullptr;
}

USoundBase * UFoot::getFootPrintSound()
{
	return PhysMat != nullptr ? PhysMat->FootstepSound : nullptr;
}
