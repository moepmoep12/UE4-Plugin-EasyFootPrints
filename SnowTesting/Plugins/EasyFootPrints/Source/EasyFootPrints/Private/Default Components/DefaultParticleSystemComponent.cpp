// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UDefaultParticleSystemComponent::spawnParticleEmitter(FVector Location, float TessellationHeight , UParticleSystem* ParticleEffect) {

	if (!ParticleEffect) {
		return;
	}

	//FVector SpawnLocation = FVector(Location.X, Location.Y, Location.Z + 0.5* TessellationHeight);
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, Location);
}


