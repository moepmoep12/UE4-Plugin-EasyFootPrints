// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSoundComponent.generated.h"

/** This is an abstract base class for a Sound Component.
*   It is responsible for playing a sound when a foot hits the ground
*/

UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBaseSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/* Plays a sound at a given location
	@Location: the location to play the sound
	@Sound: the sound that will be played	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Play Footprint Sound at Location", Keywords = "play sound footprint plugin"), Category = "EasyFootPrints")
		void playFootPrintSound(FVector Location, USoundBase* Sound);

	virtual void playFootPrintSound_Implementation(FVector Location, USoundBase* Sound) { playFootPrintSound(Location,Sound); };



};
