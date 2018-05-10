// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BaseSoundComponent.h"
#include "DefaultSoundComponent.generated.h"

/**
 * 
 */
UCLASS()
class UDefaultSoundComponent : public UBaseSoundComponent
{
	GENERATED_BODY()
	
public:
	void playFootPrintSound_Implementation(FVector Location, USoundBase* Sound) override;
	
	
};
