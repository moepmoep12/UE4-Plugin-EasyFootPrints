// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "Default Components/DefaultMovementAdjustmentComp.h"
#include "Default Components/DefaultRenderTargetComponent.h"
#include "Default Components/DefaultParticleSystemComponent.h"
#include "Default Components/DefaultPollutionComponent.h"
#include "Default Components/DefaultSoundComponent.h"
#include "FootPrintComponent.generated.h"

/* Forward Declaration */
class USoundManager;


UCLASS(ClassGroup = (EasyFootPrints), meta = (BlueprintSpawnableComponent))
class EASYFOOTPRINTS_API UFootPrintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// The array represents all bones that should be tracked by this plugin, insert bone names here
	UPROPERTY(BluePrintReadWrite, EditAnyWhere, Category = Bones)
		TArray<FName> BoneNames;

	// The name of the bone that is the centre of the skeleton
	// This is used to determine if a foot is left/right
	// default = "pelvis"
	UPROPERTY(BluePrintReadWrite, EditAnyWhere, Category = Bones)
		FName CentralBone = "pelvis";

	// The texture that is used for creating PollutionFootPrints
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = PollutionFootPrints)
		UTexture* PollutionFootPrintTexture;

	// The scaling of the PollutionFootPrints
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = PollutionFootPrints)
		FVector PollutionFootPrintsScale = FVector(1, 1, 1);

	// The texture defines the shape of the footprints that are deforming the terrain
	// Where white = no deformation, black = full deformation
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintShape)
		UTexture* FootPrintShapeTexture;

	// A transform that affects the shape of the footprints
	// Only Scaling is currently supported
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintShape)
		FTransform FootPrintShapeTransform;

	/*
	/	the classes for the components are defined here
	/	custom components can be set via blueprint
	*/
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintComponents)
		TSubclassOf<class UBaseRenderTargetComponent> RenderTargetComponent = UDefaultRenderTargetComponent::StaticClass();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintComponents)
		TSubclassOf<class UBaseMovementAdjustmentComponent> AdjMovementComponent = UDefaultMovementAdjustmentComp::StaticClass();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintComponents)
		TSubclassOf<class UBaseParticleSystemComponent> ParticleSystemComponent = UDefaultParticleSystemComponent::StaticClass();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintComponents)
		TSubclassOf<class UBasePollutionComponent> PollutionComponent = UDefaultPollutionComponent::StaticClass();

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintComponents)
		TSubclassOf<class UBaseSoundComponent> SoundComponent = UDefaultSoundComponent::StaticClass();


private:
	// the owner of this component
	UPROPERTY()
		ACharacter* Player;

	// this represents the foot that is closest to the ground
	UPROPERTY()
		UFoot* FootOnGround;

	// Array of UFoot that represents all feet that will be tracked
	UPROPERTY()
		TArray<UFoot*> TrackedFeet;

	// the location of the central bone
	UPROPERTY()
		FVector CentralBoneLocation;

	// the material of the footprint shape, reference will be set in constructor
	UPROPERTY()
		UMaterialInterface* FootPrintShapeMaterial;

	// the material for creating PollutionFootPrints, reference will be set in contructor 
	UPROPERTY()
		UMaterialInterface* PollutionFootPrintMaterial;

	/** the following components are used for several calculations and will be created from the corresponding component class */
	UPROPERTY()
		UBaseSoundComponent* SoundComputations;
	UPROPERTY()
		UBaseRenderTargetComponent* RenderTargetComputations;
	UPROPERTY()
		UBaseMovementAdjustmentComponent* MovementComputations;
	UPROPERTY()
		UBaseParticleSystemComponent* ParticleSystem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void LoadFootPositions();
	void initFeet();
	void setFootMaterials();
	void Trace();
	void setFootOnGround();
	void drawOnRenderTarget();
	void initComponents();
	void emittParticleEffect();
	void playFootPrintSound();
	void adjustMovement();
	void emittPolutionParticleEffect();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Constructor
	UFootPrintComponent();

	/* This represents the main function of the plugin. It should be called whenever a foot touches the ground */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create FootPrints", Keywords = "print plugin"), Category = "EasyFootPrints")
		void OnFootDown();

	UFUNCTION()
		UFoot* getFootOnGround() { return FootOnGround; };
	UFUNCTION()
		ACharacter* getPlayer() { return Player; };


};