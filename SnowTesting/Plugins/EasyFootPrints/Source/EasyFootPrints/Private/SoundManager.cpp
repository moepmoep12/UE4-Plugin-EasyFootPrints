// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundManager.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

/// <summary>Finds the sound for the footprint and saves it for playing </summary>
USoundManager::USoundManager()
{
	/*
	//have to find all sounds and save them
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


/// <summary>Initializes the footprint sound. </summary>


void USoundManager::InitFootprintSound(ACharacter* Player)
{

	FootprintSoundComponent->bAutoActivate = false;
	FootprintSoundComponent->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	FootprintSoundComponent->SetRelativeLocation(FVector(0.0f, -100.0f, 0.0f));
	FootprintSoundComponent->SetVolumeMultiplier(4.0);

	if (FootprintSoundCue->IsValidLowLevelFast()) {
		FootprintSoundComponent->SetSound(FootprintSoundCue);
	}
}


/// <summary>Checks if the player is on a new material and then changes the footprint sound. </summary>

void USoundManager::ChangeFootprintSound(FString NameOfNewSound)
{
	//Change FootprintSoundCue to the right sound for the material under the player
	if (FootprintSoundCue != nullptr) {
		FootprintSoundBase = FootprintSoundCue;
	}
}


/// <summary>Plays the sound for the foot touching the ground. </summary>

void USoundManager::PlayFootprintSoundALT()
{
	if (FootprintSoundComponent != nullptr) {
		FootprintSoundComponent->Play();
	}
}

/// <summary>Plays the sound for the foot touching the ground. </summary>

void USoundManager::PlayFootprintSound(FVector SoundLocation) {
	if (FootprintSoundBase != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PlayedSound"))
		float VolumeMultiplier = 4.0;
		float PitchMultiplier = 1.0;
		UGameplayStatics::PlaySoundAtLocation(this, FootprintSoundBase, SoundLocation, VolumeMultiplier, PitchMultiplier);
	}
}





