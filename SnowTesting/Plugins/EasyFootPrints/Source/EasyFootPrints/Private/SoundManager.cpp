// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundManager.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Foot.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "PhysMaterial_EasyFootPrints.h"

/// <summary>Finds the sound for the footprint and saves it for playing </summary>
USoundManager::USoundManager()
{
	/*
	//have to find the idle sound and save it 
	static ConstructorHelpers::FObjectFinder<USoundCue> FootprintSound(TEXT("SoundCue'/Game/FootprintSnowSound.FootprintSnowSound'"));
	if (!FootprintSound.Object) {
		UE_LOG(LogTemp, Warning, TEXT("FootprintSound == nullptr"))
		return;
	}
	FootprintSoundCue = FootprintSound.Object;
	FootprintSoundBase = FootprintSoundCue;
	FootprintSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootprintSoundComponent"));
	*/
}

void USoundManager::PlayFootprintSound(UFoot* FootOnGround, const UObject* Context) {
	if (FootOnGround != nullptr) {
		FVector Location = FootOnGround->getHitresult()->Location;
		UMaterialInterface* Material = FootOnGround->getHitMaterial();
		if (Material != nullptr) {
			UPhysMaterial_EasyFootPrints* PhysMat = Cast<UPhysMaterial_EasyFootPrints>(Material->GetPhysicalMaterial());
			USoundBase* Sound = PhysMat->FootstepSound;
			if (Sound != nullptr) {
				UGameplayStatics::PlaySoundAtLocation(Context, Sound, Location);
			}
		}

	}
}

void USoundManager::PlayFootprintSoundWithPollution(UFoot* FootOnGround, const UObject* Context) {
	if (FootOnGround != nullptr) {
		FVector Location = FootOnGround->getHitresult()->Location;
		//Play idle Sound for floor without material
	}
}







