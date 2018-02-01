// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RenderTargetValues.h"
#include "Default Components/DefaultPollutionComponent.h"
#include "Foot.generated.h"

/**
 * 
 */
class UTextureRenderTarget2D;
class UPhysMaterial_EasyFootPrints;

UCLASS()
class UFoot : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
		FVector Location;

	UPROPERTY()
		FRotator Rotation;

	UPROPERTY()
		FName Name;

	UPROPERTY()
		FHitResult Hitresult = FHitResult(ForceInit);

	UPROPERTY()
		UPhysMaterial_EasyFootPrints* PhysMat;

	UPROPERTY()
		FRenderTargetValues RenderTargetValues = FRenderTargetValues();

	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial = nullptr;

	UPROPERTY()
		UBasePollutionComponent* PollutionComponent;

	UPROPERTY()
		UMaterialInterface* HitMaterial;

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
	USoundBase* getFootPrintSound();

	void setLocation(FVector loc) { Location = loc; }
	void setRotation(FVector ForwardVector);
	void setBoneName(FName name) { Name = name; }
	
	void IncreaseFootPollution();
	bool hasHitComponentRenderTarget();
	void updateHitMaterial();
	void initPollutionComponent(TSubclassOf<UBasePollutionComponent> PollutionComp);
	void createPollutionFootPrint(UMaterialInterface* Material, UWorld* world);
	
};
