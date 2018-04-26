// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BaseParticleSystemComponent.h"
#include "DefaultParticleSystemComponent.generated.h"

/**
*
*/
UCLASS()
class UDefaultParticleSystemComponent : public UBaseParticleSystemComponent
{
	GENERATED_BODY()


public:
	virtual void spawnParticleEmitter(FVector Location, float TessellationHeight, UParticleSystem* ParticleEffect) override;

	void spawnPollutionParticleEffect(FVector Location, float Pollution, UParticleSystem* ParticleEffect) override;

};
