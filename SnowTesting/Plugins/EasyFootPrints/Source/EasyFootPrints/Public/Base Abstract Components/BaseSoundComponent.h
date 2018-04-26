// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSoundComponent.generated.h"

/** This is an abstract base class for a Sound Component.
*   It's responsible for playing a sound when a foot hits the ground
*/

UCLASS( ClassGroup=(EasyFootPrints), abstract )
class EASYFOOTPRINTS_API UBaseSoundComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};

	/* Plays a sound at a given location
		@Location: the location to play the sound
		@Sound: the sound that will be played	*/
	virtual void playFootPrintSound(FVector Location, USoundBase* Sound) {};

		
	
};
