// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultSoundComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UDefaultSoundComponent::playFootPrintSound_Implementation(FVector Location, USoundBase* Sound)
{
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(this, Sound, Location);
	}
}

