// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "PhysMaterial_EasyFootPrints.generated.h"

/**
 * 
 */
UCLASS()
class EASYFOOTPRINTS_API UPhysMaterial_EasyFootPrints : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		UParticleSystem* ParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EasyFootPrints)
		USoundCue* FootstepSound;
	
};
