// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "PhysMaterial_EasyFootPrints.generated.h"

/**
* An extension from the orignal PhysicalMaterial with more parameters such as ParticleEffect and Sound
* The physical material is assigned to a MaterialInstance of the MasterMaterial to have access to its ParticleEffect and SoundCue at Runtime
*/
UCLASS()
class EASYFOOTPRINTS_API UPhysMaterial_EasyFootPrints : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	/* The ParticleEffect that should be played when a foot hits the ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		UParticleSystem* ParticleSystem;

	/* The Pollution ParticleEffect that should be played when a foot is polluted by that material and hits a material without tessellation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		UParticleSystem* PollitionParticleSystem;

	/* The sound that will be played when a foot hits the ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		USoundCue* FootstepSound;

	/* The color of the pollution footprints*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		FLinearColor PollutionColor;

	/* The factor that determines how fast feet will get polluted when walking through the material */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints, meta = (ClampMin = "0.0", ClampMax = "10.0"))
		float PollutionFactor;

};
