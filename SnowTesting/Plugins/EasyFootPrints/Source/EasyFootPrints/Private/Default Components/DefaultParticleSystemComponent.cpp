// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


void UDefaultParticleSystemComponent::spawnParticleEmitter_Implementation(FVector Location, float TessellationHeight, UParticleSystem* ParticleEffect) {

	if (!ParticleEffect) {
		return;
	}

	//FVector SpawnLocation = FVector(Location.X, Location.Y, Location.Z + 0.5* TessellationHeight);
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, Location);
}

void UDefaultParticleSystemComponent::spawnPollutionParticleEffect_Implementation(FVector Location, float Pollution, UParticleSystem * ParticleEffect)
{
	if (!ParticleEffect) {
		return;
	}
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, Location);

}

