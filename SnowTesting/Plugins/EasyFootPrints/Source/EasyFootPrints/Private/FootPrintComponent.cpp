// Fill out your copyright notice in the Description page of Project Settings.

#include "FootPrintComponent.h"
#include "FootValues.h"
#include "Foot.h"
#include "GameFramework/Character.h"
#include "Engine/DecalActor.h"
#include "FootPrintDecal.h"
#include "Engine/GameEngine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "SoundManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Default Components/DefaultRenderTargetComponent.h"
#include "Default Components/DefaultMovementAdjustmentComp.h"
#include "Default Components/DefaultParticleSystemComponent.h"
#include "PhysMaterial_EasyFootPrints.h"


// Sets default values for this component's properties
UFootPrintComponent::UFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FootValues = FFootPrintValues();
	SoundManager = NewObject<USoundManager>();

}


// Called when the game starts
void UFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();

		if (!Player) {
			Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
		}

	initComponents();
	initFeet();
}

void UFootPrintComponent::initComponents()
{
	RenderTargetComputations = NewObject<UBaseRenderTargetComponent>(this, RenderTargetComponent);
	MovementComputations = NewObject<UBaseMovementAdjustmentComponent>(this, AdjMovementComponent);
	ParticleSystem = NewObject<UBaseParticleSystemComponent>(this, ParticleSystemComponent);
	MovementComputations->initComponent(this);
}


// Called every frame
void UFootPrintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/// <summary> Called when a foot touches the ground </summary> 
void UFootPrintComponent::OnFootDown()
{
	if (!Player) {
		Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	}

	LoadFootPositions();
	Trace();
	setFootOnGround();

	if (FootOnGround->hasHitComponentRenderTarget())
	{
		drawOnRenderTarget();
		FootOnGround->IncreaseFootPollution();
		MovementComputations->adjustMovement();
		emittParticleEffect();
		SoundManager->PlayFootprintSound(FootOnGround, this);
	}

	else
	{
		MovementComputations->resetMovement();

		if (FootOnGround->HasPollution())
		{
			CreatePollutionFootPrint();
			emittParticleEffect();
			SoundManager->PlayFootprintSoundWithPollution(FootOnGround, this);
		}
	}

}

void UFootPrintComponent::drawOnRenderTarget()
{
	FRenderTargetValues* RenderTargetValues = FootOnGround->getRenderTargetValues();
	RenderTargetComputations->drawOnRenderTarget(M_Spot, RenderTargetValues);
}


/// <summary>  Calls the initFeet() method and sets location of every foot </summary> 
void UFootPrintComponent::LoadFootPositions()
{

	for (UFoot* f : FootValues.TrackedFeet)
	{
		f->setLocation(Player->GetMesh()->GetBoneLocation(f->getBoneName()));
	}

	FootValues.setForwardVector(Player->GetActorForwardVector());

}


/// <summary> creates an Array of Feet with the specified Bones </summary>
void UFootPrintComponent::initFeet()
{
	if (BoneNames.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Bone Amount is <= 0"))
			return;
	}

	if (FootValues.TrackedFeet.Num() != BoneNames.Num())
	{
		FootValues.TrackedFeet.Empty();

		for (FName name : BoneNames)
		{
			UFoot* foot = NewObject<UFoot>();
			foot->setBoneName(name);
			FootValues.TrackedFeet.Add(foot);
		}
	}
}


/// <summary> Performs a Linetrace for every foot and set its rotation </summary>
void UFootPrintComponent::Trace()
{
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("FootPrint Trace"), true, Player);
	TraceParams.bReturnPhysicalMaterial = true;

	for (UFoot* f : FootValues.TrackedFeet)
	{

		FVector end = FVector(f->getLocation().X, f->getLocation().Y, f->getLocation().Z - 50.0f);
		//perform LineTrace
		GetWorld()->LineTraceSingleByChannel(*f->getHitresult(), f->getLocation(), end, ECollisionChannel::ECC_Visibility, TraceParams);
		//set Rotation
		f->setRotation(FootValues);
	}
}



/*
*	Calculates the foot that is touching/closest to the ground
*	To-Do: Implement Lambda for Sorting
*/
void UFootPrintComponent::setFootOnGround()
{
	//FootValues.TrackedFeet.Sort([](UFoot* f1, UFoot* f2) { return f1->getLocation().Z > f2->getLocation().Z; });
	int32 temp = MAX_int32;
	int32 key = 0;

	for (int32 i = 0; i < FootValues.TrackedFeet.Num(); i++)
	{
		if (FootValues.TrackedFeet[i]->getLocation().Z <= temp) {
			temp = FootValues.TrackedFeet[i]->getLocation().Z;
			key = i;
		}
	}
	FootOnGround = FootValues.TrackedFeet[key];
	FootOnGround->updateHitMaterial();
}

void UFootPrintComponent::CreatePollutionFootPrint()
{
	if (!GetWorld())
	{
		return;
	}
	ADecalActor* Adecal = GetWorld()->SpawnActor<ADecalActor>(AFootPrintDecal::StaticClass(), FootOnGround->getHitresult()->Location, FootOnGround->getRotation());
	Adecal->SetDecalMaterial(FootPrintMaterial);
	UMaterialInstanceDynamic* matinstance = Adecal->CreateDynamicMaterialInstance();
	Adecal->SetDecalMaterial(matinstance);
	FLinearColor ColorA = FLinearColor(0, 0, 0, 0);
	FLinearColor LerpedColor = UKismetMathLibrary::LinearColorLerp(ColorA, FootOnGround->getBaseColor(), FootOnGround->getFootPollution());

	matinstance->SetVectorParameterValue(FName("FootPrintColor"), LerpedColor);

	// called after FootPrints have been created
	FootOnGround->DecreaseFootPollution();

}

void UFootPrintComponent::emittParticleEffect()
{
	FVector Location = FootOnGround->getLocation();
	float height = FootOnGround->getTessellationHeight();
	ParticleSystem->spawnParticleEmitter(Location, height, FootOnGround->getParticleEffect());
}

/*
void UFootPrintComponent::EmittingParticleEffect(FVector Location) {
	float tessellationHeight = FootOnGround->getTessellationHeight();
	FVector FPPLocation = FVector(Location.X, Location.Y, Location.Z + tessellationHeight);

	UMaterialInterface* Material = FootOnGround->getHitMaterial();
	UPhysMaterial_EasyFootPrints* PhysMat = Cast<UPhysMaterial_EasyFootPrints>(Material->GetPhysicalMaterial());
	CurrentFootprintParticleSystem = PhysMat->ParticleSystem;


	if (CurrentFootprintParticleSystem != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(this, CurrentFootprintParticleSystem, FPPLocation);
	}
}

void UFootPrintComponent::EmittingParticleEffectWithPollution(FVector Location) {
	if (CurrentFootprintParticleSystem != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(this, CurrentFootprintParticleSystem, Location);
	}
}
*/
