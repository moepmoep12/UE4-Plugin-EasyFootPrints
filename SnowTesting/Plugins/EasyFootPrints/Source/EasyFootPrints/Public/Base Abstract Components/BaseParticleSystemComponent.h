
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseParticleSystemComponent.generated.h"


/* The abstract base class for ParticleSystemComponents. It spawns particle systems when walking on tessellated material and also pollution particle effects.
* The class can be parent to blueprints and methods can be overriden in blueprint
*/

UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBaseParticleSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Spawns a particle effect at given location
	*@Location: the location of the foot
	*@TessellationHeight: the tessellationheight of the material, can be used to adjust Z-position
	*@ParticleEffect: the particle effect to be spawned
	*/
	UFUNCTION(BluePrintNativeEvent, meta = (DisplayName = "Spawn Particle Effect at Location", Keywords = "particle emitter footprint plugin"), Category = "EasyFootPrints")
		void spawnParticleEmitter(FVector Location, float TessellationHeight, UParticleSystem* ParticleEffect);

	virtual void spawnParticleEmitter_Implementation(FVector Location, float TessellationHeight, UParticleSystem* ParticleEffect) { 
		spawnParticleEmitter(Location,TessellationHeight,ParticleEffect); 
	};

	/** this will be called when a foot has pollution and hits a material without tessellation
	@Location: the location of the foot
	@Pollution: the amount of pollution, can be used to adjust the particle effect
	@ParticleEffect: the particle effect to be spawned
	*/
	UFUNCTION(BluePrintNativeEvent, meta = (DisplayName = "Spawn Pollution Particle Effect at Location", Keywords = "particle emitter footprint plugin"), Category = "EasyFootPrints")
		void spawnPollutionParticleEffect(FVector Location, float Pollution, UParticleSystem* ParticleEffect);

	virtual void spawnPollutionParticleEffect_Implementation(FVector Location, float Pollution, UParticleSystem* ParticleEffect) { 
		spawnPollutionParticleEffect(Location,Pollution,ParticleEffect);
	};


};
