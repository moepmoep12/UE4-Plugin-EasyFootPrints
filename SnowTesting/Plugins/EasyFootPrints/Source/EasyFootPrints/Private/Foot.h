// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RenderTargetValues.h"
#include "Default Components/DefaultPollutionComponent.h"
#include "Foot.generated.h"

/** This class represents a foot, for every specified bone its own foot instance will be created
*
*/
class UTextureRenderTarget2D;
class UPhysMaterial_EasyFootPrints;

UCLASS()
class UFoot : public UObject
{
	GENERATED_BODY()

private:
	// the world location of the foot
	UPROPERTY()
		FVector Location;

	// the world rotation of the foot
	UPROPERTY()
		FRotator Rotation;

	// the name of the foot
	UPROPERTY()
		FName Name;

	// The hitresult that was created by the trace
	UPROPERTY()
		FHitResult Hitresult = FHitResult(ForceInit);

	// the physical material of the material that was hit
	UPROPERTY()
		UPhysMaterial_EasyFootPrints* PhysMat;

	// a struct for several values needed for drawing onto render target
	UPROPERTY()
		FRenderTargetValues RenderTargetValues = FRenderTargetValues();

	// the render target of the hit material
	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial = nullptr;

	// the pollution component that is responsible for creating decals
	UPROPERTY()
		UBasePollutionComponent* PollutionComponent;

	// the material that was hit by the trace
	UPROPERTY()
		UMaterialInterface* HitMaterial;

	// the material that will be used for creating decals
	UPROPERTY()
		UMaterialInstanceDynamic* PollutionMaterial;

	// the material that will be used for drawing onto render target
	UPROPERTY()
		UMaterialInstanceDynamic* FootPrintShapeInstance;

protected:
	void updateRenderTargetValues();


public:
	const FVector getLocation() { return Location; }
	FHitResult* getHitresult() { return &Hitresult; }
	const FName getBoneName() { return Name; };
	bool const HasPollution() { return PollutionComponent->hasPollution(); }
	float getDepth();
	float getTessellationHeight();
	FRenderTargetValues* getRenderTargetValues();
	UParticleSystem* getParticleEffect();
	UParticleSystem* getPollutionParticleEffect();
	USoundBase* getFootPrintSound();
	UMaterialInstanceDynamic* getFootPrintShape() { return FootPrintShapeInstance; };

	void setLocation(FVector loc) { Location = loc; }
	void setRotation(FVector ForwardVector);
	void setBoneName(FName name) { Name = name; }
	void setFootPrintShape(UMaterialInstanceDynamic* FootPrintShape) { FootPrintShapeInstance = FootPrintShape; };
	void setPollutionMaterial(UMaterialInstanceDynamic* PollutionMat) { PollutionMaterial = PollutionMat; };

	void IncreaseFootPollution();
	bool hasHitComponentRenderTarget();
	void updateHitMaterial();
	void initPollutionComponent(TSubclassOf<UBasePollutionComponent> PollutionComp);
	void createPollutionFootPrint(UWorld* world, FVector Scale = FVector(1, 1, 1));

};
