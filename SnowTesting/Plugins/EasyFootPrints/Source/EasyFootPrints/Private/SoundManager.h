// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class USoundManager : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		UAudioComponent* FootprintSoundComponent;
	UPROPERTY()
		USoundCue* FootprintSoundCue;
	UPROPERTY()
		USoundBase* FootprintSoundBase;

protected:

public:

	void PlayFootprintSound(UFoot* FootOnGround, const UObject* Context);
	void PlayFootprintSoundWithPollution(UFoot* FootOnGround, const UObject* Context);
	USoundManager();
	
	
};
