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
	virtual void spawnParticleEmitter(FVector Location, float TessellationHeight , UParticleSystem* ParticleEffect) {};
		
	
};
