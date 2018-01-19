// Fill out your copyright notice in the Description page of Project Settings.

#include "FootPrintComponent.h"
#include "FootValues.h"
#include "Foot.h"
#include "GameFramework/Character.h"
#include "Engine/DecalActor.h"
#include "FootPrintDecal.h"
#include "Engine/GameEngine.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SoundManager.h"
#include "LandscapeComponent.h"
#include "Runtime/Landscape/Classes/LandscapeProxy.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



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
		MovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
		OriginaMaxWalkingSpeed = MovementComponent->MaxWalkSpeed;
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

	if (hasComponentRenderTarget())
	{
		createFootPrintOnRenderTarget();
	}

	else
	{
		if (FootOnGround->HasPollution()) 
		{
			CreatePollutionFootPrint();
		}
	}
	

	//SoundManager->PlayFootprintSound();

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
		FVector end = FVector(f->getLocation().X, f->getLocation().Y, f->getLocation().Z - 20.0f);
		//perform LineTrace
		GetWorld()->LineTraceSingleByChannel(*f->getHitresult(), f->getLocation(), end, ECollisionChannel::ECC_Visibility, TraceParams);
		//set Rotation
		f->setRotation(FootValues);
	}
}



/*
*	Calculates the foot that is touching the ground
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


/*
*	TO-DO : Implement Pollution System
*/
void UFootPrintComponent::SpawnDecal()
{
	TWeakObjectPtr<UPhysicalMaterial> physmat = FootOnGround->getHitresult()->PhysMaterial;


	/*  TODO : PollutionSystem

	if (physmat != nullptr)
	{
	if (foot->ChangeMaterial(physmat.Get()) && foot->getLastMaterialTouched()->getAmountSteps() > 0)
	{
	

	}

	}
	*/
}


/*
*	Checks whether the hit component has a MaterialInstance with a RenderTarget
*	if true set RenderTargetOfHitMaterial
*/
bool UFootPrintComponent::hasComponentRenderTarget()
{
	UActorComponent* ActorComponent = nullptr;
	ULandscapeComponent* LandscapeComponent = nullptr;

	if (FootOnGround->getHitresult()->GetActor()) {
		ActorComponent = FootOnGround->getHitresult()->GetActor()->FindComponentByClass(ULandscapeComponent::StaticClass());
		LandscapeComponent = Cast<ULandscapeComponent>(ActorComponent);
	}
	
	if (!FootOnGround || !LandscapeComponent) {
		resetWalkSpeed();
		return false;
	}

	TArray<UMaterialInterface*> MaterialsOfHitComponent = TArray<UMaterialInterface*>();
	LandscapeComponent->GetUsedMaterials(MaterialsOfHitComponent);
	
	for (UMaterialInterface* mat : MaterialsOfHitComponent) {
		if (mat->GetTextureParameterValue(FName("RenderTarget"), RenderTexture)) {
			float Depth = 1;
			if (mat->GetScalarParameterValue(FName("Depth"), Depth)) {
				adjustMaxWalkSpeed(Depth);
			}
			if (RenderTexture) {
				RenderTargetOfHitMaterial = static_cast<UTextureRenderTarget2D*>(RenderTexture);
				return true;
			}
		}
	}
	resetWalkSpeed();
	return false;
}
void UFootPrintComponent::createFootPrintOnRenderTarget_Implementation()
{

	UCanvas* Canvas = nullptr;
	FVector2D ScreenSize = FVector2D(0, 0);
	FDrawToRenderTargetContext Context;
	FVector2D CoordinatePosition = FVector2D(0, 0);

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(Player, RenderTargetOfHitMaterial, Canvas, ScreenSize, Context);

	FVector2D ScreenPosition = InitComputationOfRenderTargetScreenPosition();
	ScreenSize = ComputeRenderTargetScreenSize();
	Canvas->K2_DrawMaterial(M_Spot, ScreenPosition, ScreenSize, CoordinatePosition);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(Player, Context);
	RenderTargetOfHitMaterial = nullptr;

	FootOnGround->IncreaseFootPollution();

}

FVector2D UFootPrintComponent::ComputeRenderTargetScreenSize() {
	int32 XSize = RenderTargetOfHitMaterial->SizeX;
	int32 YSize = RenderTargetOfHitMaterial->SizeY;
	FVector2D ScreenSize = FVector2D(XSize * 0.01, YSize * 0.01);
	return ScreenSize;
}

FVector2D UFootPrintComponent::InitComputationOfRenderTargetScreenPosition() {

	FVector ActorLocation = FootOnGround->getHitresult()->GetActor()->GetActorLocation();
	FVector HitLocation = FootOnGround->getHitresult()->Location;
	FVector ActorBounds;
	FVector ActorBoundsOrigin;
	FootOnGround->getHitresult()->GetActor()->GetActorBounds(true, ActorBoundsOrigin, ActorBounds);

	FVector2D ScreenPosition = ComputeScreenPositionOnRenderTarget(ActorLocation, HitLocation, ActorBounds);
	return ScreenPosition;

}


FVector2D UFootPrintComponent::ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds) {

	int32 XSizeOfRT = RenderTargetOfHitMaterial->SizeX;
	int32 YSizeOfRT = RenderTargetOfHitMaterial->SizeY;
	FVector2D RenderTargetSize = FVector2D(XSizeOfRT, YSizeOfRT);

	FVector2D ActorLocation2D = Get2DVectorWithXAndYFrom3DVector(ActorLocation);
	FVector2D HitLocation2D = Get2DVectorWithXAndYFrom3DVector(HitLocation);
	FVector2D ActorBounds2D = Get2DVectorWithXAndYFrom3DVector(ActorBounds);

	ActorBounds2D *= 2;
	FVector2D ScreenPosition = ActorLocation2D - HitLocation2D;
	ScreenPosition.X = UKismetMathLibrary::Abs(ScreenPosition.X / ActorBounds2D.X);
	ScreenPosition.Y = UKismetMathLibrary::Abs(ScreenPosition.Y / ActorBounds2D.Y);

	FVector2D ScreenPositionPart1 = FVector2D(XSizeOfRT * (ScreenPosition.X - 0.5), YSizeOfRT * (ScreenPosition.Y - 0.5));
	FVector2D ScreenPositionPart2 = FVector2D(XSizeOfRT * 0.5 * 0.99, YSizeOfRT * 0.5 * 0.99);
	ScreenPosition = ScreenPositionPart1 + ScreenPositionPart2;
	//ScreenPosition = RenderTargetSize - ScreenPosition;
	return ScreenPosition;
}

FVector2D UFootPrintComponent::Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed) {
	return FVector2D(VectorToBeComputed.X, VectorToBeComputed.Y);
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
	GEngine->AddOnScreenDebugMessage(3, 3, FColor::Red, FString("Reseting WalkSpeed"));
	MovementComponent->MaxWalkSpeed = OriginaMaxWalkingSpeed;
	CurrentMaxWalkSpeed = OriginaMaxWalkingSpeed;
	
}

void UFootPrintComponent::adjustMaxWalkSpeed(float depth)
{
	if (!MovementComponent) {
		GEngine->AddOnScreenDebugMessage(2, 3, FColor::Red, FString("MovementComponent = nullptr"));
		return;
	}

	float adjustedWalkSpeed = OriginaMaxWalkingSpeed * (1 / depth);
	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Blue,FString("Depth:") +  FString::SanitizeFloat(depth));
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Blue,FString("adjustedSpeed:" +  FString::SanitizeFloat(adjustedWalkSpeed)));

	if (adjustedWalkSpeed != CurrentMaxWalkSpeed) {
		Cast<UCharacterMovementComponent>(Player->GetMovementComponent())->MaxWalkSpeed = adjustedWalkSpeed;
		CurrentMaxWalkSpeed = adjustedWalkSpeed;
	}
}
