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
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RenderTargetComputations.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values for this component's properties
UFootPrintComponent::UFootPrintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FootValues = FFootPrintValues();
	//SoundManager = NewObject<USoundManager>();
	InitFootprintParticleSystems();

}


// Called when the game starts
void UFootPrintComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin Play"))

		if (!Player) {
			Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
			MovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
			OriginaMaxWalkingSpeed = MovementComponent->MaxWalkSpeed;
			OriginalJumpVelocity = MovementComponent->JumpZVelocity;
			RenderTargetComputations = NewObject<URenderTargetComputations>();
			RenderTargetComputations->initRenderTargetComputations(this);
		}

	//SoundManager->InitFootprintSound(Player);
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

	if (RenderTargetComputations->computeRenderTarget())
	{
		AdjustCharacterMovement();
		EmittingParticleEffect(FootOnGround->getHitresult()->Location);
	}

	else
	{
		resetWalkSpeed();
		resetJumpVelocity();

		if (FootOnGround->HasPollution())
		{
			CreatePollutionFootPrint();
			EmittingParticleEffectWithPollution(FootOnGround->getHitresult()->Location);
		}
	}

	//SoundManager->PlayFootprintSound(FootOnGround->getHitresult()->Location);
}

void UFootPrintComponent::AdjustCharacterMovement()
{
	float depth = FootOnGround->getDepth();
	adjustMaxWalkSpeed(depth);
	adjustJumpVelocity(depth);
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

void UFootPrintComponent::resetWalkSpeed()
{
	if (!MovementComponent) {
		return;
	}
	if (MovementComponent->MaxWalkSpeed != OriginaMaxWalkingSpeed) {
		//GEngine->AddOnScreenDebugMessage(3, 0.1, FColor::Red, FString("Reseting WalkSpeed"));
		MovementComponent->MaxWalkSpeed = OriginaMaxWalkingSpeed;
	}

	CurrentMaxWalkSpeed = OriginaMaxWalkingSpeed;
}

void UFootPrintComponent::resetJumpVelocity()
{
	if (!MovementComponent) {
		return;
	}

	if (MovementComponent->JumpZVelocity != CurrentJumpVelocity) {
		MovementComponent->JumpZVelocity = OriginalJumpVelocity;
	}

	CurrentJumpVelocity = OriginalJumpVelocity;
}

void UFootPrintComponent::adjustMaxWalkSpeed(float depth)
{
	if (!MovementComponent) {
		return;
	}

	float adjustedWalkSpeed = OriginaMaxWalkingSpeed * (1 / depth);

	if (adjustedWalkSpeed != CurrentMaxWalkSpeed) {
		Cast<UCharacterMovementComponent>(Player->GetMovementComponent())->MaxWalkSpeed = adjustedWalkSpeed;
		CurrentMaxWalkSpeed = adjustedWalkSpeed;
	}
}

void UFootPrintComponent::adjustJumpVelocity(float depth)
{
	if (!MovementComponent) {
		return;
	}

	float adjustedJumpVelocity = OriginalJumpVelocity * (1 / depth);
	if (adjustedJumpVelocity != CurrentJumpVelocity) {
		Cast<UCharacterMovementComponent>(Player->GetMovementComponent())->JumpZVelocity = adjustedJumpVelocity;
		CurrentJumpVelocity = adjustedJumpVelocity;
	}
}

void UFootPrintComponent::EmittingParticleEffect(FVector Location) {
	UMaterialInterface* Material = FootOnGround->getHitMaterial();
	float tessellationHeight = FootOnGround->getTessellationHeight();
	FVector FPPLocation = FVector(Location.X, Location.Y, Location.Z + tessellationHeight);
	//UE_LOG(LogTemp, Warning, TEXT("Material Name is %s"), *Material->GetName());
	if (Material->GetName().Equals(FString(TEXT("LandscapeMaterialInstanceConstant_66")))) {
		if (FootprintParticleSystemSand != nullptr) {
			FootprintParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(this, FootprintParticleSystemSand, FPPLocation);
		}
	}
	else {
		if (FootprintParticleSystemSnow != nullptr) {
			FootprintParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(this, FootprintParticleSystemSnow, FPPLocation);
		}
	}
}

void UFootPrintComponent::EmittingParticleEffectWithPollution(FVector Location) {
	if (FootprintParticleSystemSnow != nullptr) {
		FootprintParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(this, FootprintParticleSystemSnow, Location);
	}
}

void  UFootPrintComponent::InitFootprintParticleSystems() {
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FootprintParticles1(TEXT("ParticleSystem'/EasyFootPrints/ParticleEffects/ImpactEffectSnow.ImpactEffectSnow'"));
	if (!FootprintParticles1.Object) {
		UE_LOG(LogTemp, Warning, TEXT("FootprintParticleSnow == nullptr"))
			return;
	}
	FootprintParticleSystemSnow = FootprintParticles1.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FootprintParticles2(TEXT("ParticleSystem'/EasyFootPrints/ParticleEffects/ImpactEffectSand.ImpactEffectSand'"));
	if (!FootprintParticles2.Object) {
		UE_LOG(LogTemp, Warning, TEXT("FootprintParticleSand == nullptr"))
			return;
	}
	FootprintParticleSystemSand = FootprintParticles2.Object;
}

