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
	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		TArray<FName> BoneNames;

	// The material that will be used when creating pollution footprints
	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		UMaterialInterface* PollutionFootPrintMaterial;

	// The material which defines the shape that will deform the landscape
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = FootPrintShape)
		UMaterialInterface* FootPrintShapeMaterial;

	// A transform that affects the shape of the footprints
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create FootPrints", Keywords = "print plugin"), Category = "EasyFootPrints")
		void OnFootDown();
	UFUNCTION()
		UFoot* getFootOnGround() { return FootOnGround; };
	UFUNCTION()
		ACharacter* getPlayer() { return Player; };


};