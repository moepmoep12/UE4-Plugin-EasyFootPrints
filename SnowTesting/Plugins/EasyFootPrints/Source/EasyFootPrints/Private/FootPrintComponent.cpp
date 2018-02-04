// Fill out your copyright notice in the Description page of Project Settings.

#include "FootPrintComponent.h"
#include "Foot.h"
#include "GameFramework/Character.h"
#include "Engine/GameEngine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Default Components/DefaultRenderTargetComponent.h"
#include "Default Components/DefaultMovementAdjustmentComp.h"
#include "Default Components/DefaultParticleSystemComponent.h"
#include "PhysMaterial_EasyFootPrints.h"


// Sets default values for this component's properties
UFootPrintComponent::UFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!Player) {
		Player = Cast<ACharacter>(GetOwner());
	}

	initComponents();
	initFeet();
}

/** creates components from the specified class; 
	* if not specified, default components will be used */
void UFootPrintComponent::initComponents()
{
	RenderTargetComputations = NewObject<UBaseRenderTargetComponent>(this, RenderTargetComponent);
	MovementComputations = NewObject<UBaseMovementAdjustmentComponent>(this, AdjMovementComponent);
	ParticleSystem = NewObject<UBaseParticleSystemComponent>(this, ParticleSystemComponent);
	SoundComputations = NewObject<UBaseSoundComponent>(this, SoundComponent);
	MovementComputations->initComponent(Cast<UCharacterMovementComponent>(Player->GetMovementComponent()));
}


// Called every frame
void UFootPrintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/**	This represents the main loop of the plugin
	* it will be called whenever a foot touches the ground */
void UFootPrintComponent::OnFootDown()
{
	if (!Player) {
		Player = Cast<ACharacter>(GetOwner());
	}

	LoadFootPositions();
	Trace();
	setFootOnGround();

	if (FootOnGround->hasHitComponentRenderTarget())
	{
		drawOnRenderTarget();
		FootOnGround->IncreaseFootPollution();
		adjustMovement();
		emittParticleEffect();
		playFootPrintSound();
	}

	else
	{
		MovementComputations->resetMovement();

		if (FootOnGround->HasPollution())
		{

			FootOnGround->createPollutionFootPrint(PollutionFootPrintMaterial, GetWorld());
			emittPolutionParticleEffect();
			//SoundManager->PlayFootprintSoundWithPollution(FootOnGround, this);

		}
	}

}

/**	Calls the drawOnRenderTarget method from the RenderTargetComponent with the specified parameters */
void UFootPrintComponent::drawOnRenderTarget()
{
	FRenderTargetValues* RenderTargetValues = FootOnGround->getRenderTargetValues();
	RenderTargetComputations->drawOnRenderTarget(MaterialToDrawOnRenderTarget, RenderTargetValues);
}

/** Sets location of every foot based on the bone position	*/ 
void UFootPrintComponent::LoadFootPositions()
{
	for (UFoot* f : TrackedFeet)
	{
		f->setLocation(Player->GetMesh()->GetBoneLocation(f->getBoneName()));
	}
}

/** Creates an array of UFoot for every foot specified in the Array BoneNames  */
void UFootPrintComponent::initFeet()
{
	if (BoneNames.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Bone Amount is <= 0"))
			return;
	}

	if (TrackedFeet.Num() != BoneNames.Num())
	{
		TrackedFeet.Empty();

		for (FName name : BoneNames)
		{
			UFoot* foot = NewObject<UFoot>();
			foot->setBoneName(name);
			foot->initPollutionComponent(PollutionComponent);
			TrackedFeet.Add(foot);
		}
	}
}

/**  Performs a LineTraceSingleyByChannel for every foot in the UFoot Array*/
void UFootPrintComponent::Trace()
{
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("FootPrint Trace"), true, Player);
	TraceParams.bReturnPhysicalMaterial = true;

	for (UFoot* f : TrackedFeet)
	{

		FVector end = FVector(f->getLocation().X, f->getLocation().Y, f->getLocation().Z - 50.0f);
		//perform LineTrace
		GetWorld()->LineTraceSingleByChannel(*f->getHitresult(), f->getLocation(), end, ECollisionChannel::ECC_Visibility, TraceParams);
		//set Rotation
		f->setRotation(Player->GetActorForwardVector());
	}
}

/** Calculates the foot that is closest to the ground and set it to FootOnGround
	* To-Do: Implement Lambda for Sorting */
void UFootPrintComponent::setFootOnGround()
{
	//FootValues.TrackedFeet.Sort([](UFoot* f1, UFoot* f2) { return f1->getLocation().Z > f2->getLocation().Z; });
	int32 temp = MAX_int32;
	int32 key = 0;

	for (int32 i = 0; i < TrackedFeet.Num(); i++)
	{
		if (TrackedFeet[i]->getLocation().Z <= temp) {
			temp = TrackedFeet[i]->getLocation().Z;
			key = i;
		}
	}
	
	FootOnGround = TrackedFeet[key];
	FootOnGround->updateHitMaterial();
}

/** Calls the spawnParticleEmitter method of the ParticleComponent with the specified parameters */
void UFootPrintComponent::emittParticleEffect()
{
	FVector Location = FootOnGround->getLocation();
	float height = FootOnGround->getTessellationHeight();
	ParticleSystem->spawnParticleEmitter(Location, height, FootOnGround->getParticleEffect());
}

void UFootPrintComponent::emittPolutionParticleEffect()
{
	FVector Location = FootOnGround->getLocation();
	ParticleSystem->spawnPollutionParticleEffect(Location, 0, FootOnGround->getPollutionParticleEffect());
}

/** Calls the playFootPrindSound method from the SoundComponent with the specified parameters*/
void UFootPrintComponent::playFootPrintSound()
{
	FVector Location = FootOnGround->getLocation();
	USoundBase* Sound = FootOnGround->getFootPrintSound();
	SoundComputations->playFootPrintSound(Location, Sound);
}

/** Calls the adjustMovement method of the MovementComponent after getting depth from FootOnGround */
void UFootPrintComponent::adjustMovement()
{
	float depth = FootOnGround->getDepth();
	MovementComputations->adjustMovement(depth);
}


