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
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SoundManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Default Components/DefaultRenderTargetComponent.h"
#include "Default Components/DefaultMovementAdjustmentComp.h"
#include "PhysMaterial_EasyFootPrints.h"


// Sets default values for this component's properties
UFootPrintComponent::UFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FootValues = FFootPrintValues();
	//SoundManager = NewObject<USoundManager>();

}


// Called when the game starts
void UFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin Play"))

		if (!Player) {
			Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
		}

	initComponents();
	//SoundManager->InitFootprintSound(Player);
}

void UFootPrintComponent::initComponents()
{
	if (!RenderTargetComponent) {
		RenderTargetComputations = NewObject<UDefaultRenderTargetComponent>();
	}

	if (!AdjMovementComponent) {
		MovementComputations = NewObject<UDefaultMovementAdjustmentComp>();
		MovementComputations->initComponent(this);
	}
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

	if (RenderTargetComputations->drawOnRenderTarget(this))
	{
		MovementComputations->adjustMovement();
		EmittingParticleEffect(FootOnGround->getHitresult()->Location);
	}

	else
	{
		MovementComputations->resetMovement();

		if (FootOnGround->HasPollution())
		{
			CreatePollutionFootPrint();
			EmittingParticleEffectWithPollution(FootOnGround->getHitresult()->Location);
		}
	}

	//SoundManager->PlayFootprintSound(FootOnGround->getHitresult()->Location);
}




/// <summary>  Calls the initFeet() method and sets location of every foot </summary> 
void UFootPrintComponent::LoadFootPositions()
{
	if (BoneNames.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Bone Amount is <= 0"))
			return;
	}

	if (FootValues.TrackedFeet.Num() != BoneNames.Num())
	{
		initFeet();
	}

	for (UFoot* f : FootValues.TrackedFeet)
	{
		f->setLocation(Player->GetMesh()->GetBoneLocation(f->getBoneName()));
	}

	FootValues.setForwardVector(Player->GetActorForwardVector());

}


/// <summary> creates an Array of Feet with the specified Bones </summary>
void UFootPrintComponent::initFeet()
{
	FootValues.TrackedFeet.Empty();

	for (FName name : BoneNames)
	{
		UFoot* foot = NewObject<UFoot>();
		foot->setBoneName(name);
		FootValues.TrackedFeet.Add(foot);
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


void UFootPrintComponent::EmittingParticleEffect(FVector Location) {
	UMaterialInterface* Material = FootOnGround->getHitMaterial();
	float tessellationHeight = FootOnGround->getTessellationHeight();
	FVector FPPLocation = FVector(Location.X, Location.Y, Location.Z + tessellationHeight);
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

