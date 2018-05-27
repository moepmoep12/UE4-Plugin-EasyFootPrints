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
#include "Materials/MaterialInstanceDynamic.h"
#include "ConstructorHelpers.h"
#include "PhysMaterial_EasyFootPrints.h"


// Sets default values for this component's properties
UFootPrintComponent::UFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	/// Find the FootPrintShape Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/EasyFootPrints/Materials/FootPrintShape.FootPrintShape'"));
	/// Find the PollutionFootPrint Material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> PollutionMaterial(TEXT("Material'/EasyFootPrints/Materials/PollutionMaterial.PollutionMaterial'"));

	if (Material.Object != NULL)
	{
		FootPrintShapeMaterial = (UMaterialInterface*)Material.Object;
	}
	if (PollutionMaterial.Object != NULL) {
		PollutionFootPrintMaterial = (UMaterialInterface*)PollutionMaterial.Object;
	}
	
}


// Called when the game starts
void UFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APawn>(GetOwner());
	PlayerMesh = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	initComponents();
	initFeet();
	setFootMaterials();
}

/** creates components from the specified class;
* if not specified, default components will be used */
void UFootPrintComponent::initComponents()
{
	RenderTargetComponent = RenderTargetComponentClass ? NewObject<UBaseRenderTargetComponent>(this, RenderTargetComponentClass) : nullptr;
	RenderTargetComponent->RegisterComponent();
	MovementComponent = AdjMovementComponentClass ? NewObject<UBaseMovementAdjustmentComponent>(this, AdjMovementComponentClass) : nullptr;
	ParticleSystemComponent = ParticleSystemComponentClass ? NewObject<UBaseParticleSystemComponent>(this, ParticleSystemComponentClass) : nullptr;
	SoundComponent = SoundComponentClass ? NewObject<UBaseSoundComponent>(this, SoundComponentClass) : nullptr;
	MovementComponent->initComponent(Player->GetMovementComponent());

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
			foot->initPollutionComponent(PollutionComponentClass);
			TrackedFeet.Add(foot);
		}
	}
}

/** Determines for every foot if its right or left and sets its material accordingly */
void UFootPrintComponent::setFootMaterials()
{
	if (!PlayerMesh) {
		return;
	}

	UMaterialInstanceDynamic* FootPrintShape_Right = UMaterialInstanceDynamic::Create(FootPrintShapeMaterial, this);
	UMaterialInstanceDynamic* FootPrintShape_Left = UMaterialInstanceDynamic::Create(FootPrintShapeMaterial, this);
	UMaterialInstanceDynamic* PollutionMaterial_Right = UMaterialInstanceDynamic::Create(PollutionFootPrintMaterial, this);
	UMaterialInstanceDynamic* PollutionMaterial_Left = UMaterialInstanceDynamic::Create(PollutionFootPrintMaterial, this);
	// Mirror the texture on left feet
	FootPrintShape_Left->SetScalarParameterValue(FName("Mirror"), -1.0f);
	PollutionMaterial_Left->SetScalarParameterValue(FName("Mirror"), -1.0f);
	// Set FootPrintShapeTexture
	FootPrintShape_Left->SetTextureParameterValue(FName("Texture"), FootPrintShapeTexture);
	FootPrintShape_Right->SetTextureParameterValue(FName("Texture"), FootPrintShapeTexture);
	// Set PollutionTexture
	PollutionMaterial_Left->SetTextureParameterValue(FName("Texture"), PollutionFootPrintTexture);
	PollutionMaterial_Right->SetTextureParameterValue(FName("Texture"), PollutionFootPrintTexture);

	for (UFoot* f : TrackedFeet) {
		FRotator rot = Player->GetActorRotation().GetInverse();
		FVector FootLocation = PlayerMesh->GetBoneLocation(f->getBoneName());
		FVector PelvisLocation = PlayerMesh->GetBoneLocation(CentralBone);
		if (rot.RotateVector(PelvisLocation - FootLocation).X > 0) {
			f->setFootPrintShape(FootPrintShape_Right);
			f->setPollutionMaterial(PollutionMaterial_Right);
		}
		else {
			f->setFootPrintShape(FootPrintShape_Left);
			f->setPollutionMaterial(PollutionMaterial_Left);
		}
	}
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
	if (!Player || !PlayerMesh) {
		return;
	}

	LoadFootPositions();
	Trace();
	setFootOnGround();

	for (UFoot* f : FeetOnGround) {
		FootOnGround = f;
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
			MovementComponent->resetMovement();

			if (FootOnGround->HasPollution())
			{
				FootOnGround->createPollutionFootPrint(GetWorld(), PollutionFootPrintsScale);
				emittPolutionParticleEffect();
			}
		}
	}
}



/** Sets location of every foot based on the bone position	*/
void UFootPrintComponent::LoadFootPositions()
{
	if (!PlayerMesh) {
		return;
	}

	for (UFoot* f : TrackedFeet)
	{
		f->setLocation(PlayerMesh->GetBoneLocation(f->getBoneName()));
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
		f->setRotation(GetOwner()->GetActorForwardVector());
	}
}

/** Calculates the foot that is closest to the ground and set it to FootOnGround
* To-Do: Implement Lambda for Sorting */
void UFootPrintComponent::setFootOnGround()
{
	//FootValues.TrackedFeet.Sort([](UFoot* f1, UFoot* f2) { return f1->getLocation().Z > f2->getLocation().Z; });
	int32 temp = MAX_int32;
	int32 key = 0;
	float maxDifferencePercent = 0.01;

	FeetOnGround.Empty();

	for (int32 i = 0; i < TrackedFeet.Num(); i++)
	{
		if (TrackedFeet[i]->getLocation().Z <= temp) {
			temp = TrackedFeet[i]->getLocation().Z;
			key = i;
		}
	}

	UFoot* DeepestFoot = TrackedFeet[key];
	//FootOnGround->updateHitMaterial();

	for (UFoot* f : TrackedFeet) {
		float difference = FVector::Distance(DeepestFoot->getLocation(), f->getLocation());

		if (FMath::IsNearlyZero(difference, tolerance)) {
			FeetOnGround.Add(f);
			f->updateHitMaterial();
		}
	}
}


/**	Calls the drawOnRenderTarget method from the RenderTargetComponent with the specified parameters */
void UFootPrintComponent::drawOnRenderTarget()
{
	if (!RenderTargetComponent) {
		return;
	}
	FRenderTargetValues* RenderTargetValues = FootOnGround->getRenderTargetValues();
	RenderTargetComponent->drawOnRenderTarget_Implementation(FootOnGround->getFootPrintShape(), *RenderTargetValues, FootPrintShapeTransform);
}

/** Calls the spawnParticleEmitter method of the ParticleComponent with the specified parameters */
void UFootPrintComponent::emittParticleEffect()
{
	if (!ParticleSystemComponent) {
		return;
	}


	FVector Location = FootOnGround->getLocation();
	float height = FootOnGround->getTessellationHeight();
	ParticleSystemComponent->spawnParticleEmitter(Location, height, FootOnGround->getParticleEffect());
}

void UFootPrintComponent::emittPolutionParticleEffect()
{
	if (!ParticleSystemComponent) {
		return;
	}

	FVector Location = FootOnGround->getLocation();
	ParticleSystemComponent->spawnPollutionParticleEffect(Location, 0, FootOnGround->getPollutionParticleEffect());
	
}

/** Calls the playFootPrindSound method from the SoundComponent with the specified parameters*/
void UFootPrintComponent::playFootPrintSound()
{
	if (!SoundComponent) {
		return;
	}

	FVector Location = FootOnGround->getLocation();
	USoundBase* Sound = FootOnGround->getFootPrintSound();
	SoundComponent->playFootPrintSound(Location, Sound);
	
}

/** Calls the adjustMovement method of the MovementComponent after getting depth from FootOnGround */
void UFootPrintComponent::adjustMovement()
{
	if (!MovementComponent) {
		return;
	}

	float depth = FootOnGround->getDepth();
	MovementComponent->adjustMovement(depth);
}


