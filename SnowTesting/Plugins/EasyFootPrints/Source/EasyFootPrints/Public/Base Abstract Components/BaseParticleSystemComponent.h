// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseParticleSystemComponent.generated.h"


UCLASS( ClassGroup=(EasyFootPrints), abstract )
class EASYFOOTPRINTS_API UBaseParticleSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};

	/** this will be called when a foot touches a material with tessellation
		*@Location: the location of the foot
		*@TessellationHeight: the tessellationheight of the material, can be used to adjust Z-position
		*@ParticleEffect: the particle effect that belongs to the material */
	virtual void spawnParticleEmitter(FVector Location, float TessellationHeight , UParticleSystem* ParticleEffect) {};

	virtual void spawnPollutionParticleEffect(FVector Location, float Pollution, UParticleSystem* ParticleEffect) {};
		
	
};
