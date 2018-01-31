// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "FootValues.h"
#include "Default Components/DefaultMovementAdjustmentComp.h"
#include "Default Components/DefaultRenderTargetComponent.h"
#include "Default Components/DefaultParticleSystemComponent.h"
#include "FootPrintComponent.generated.h"

/* Forward Declaration */
class USoundManager;


UCLASS(ClassGroup = (EasyFootPrints), meta = (BlueprintSpawnableComponent))
class EASYFOOTPRINTS_API UFootPrintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		TArray<FName> BoneNames;
	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		UMaterialInterface* FootPrintMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
		UMaterialInterface* M_Spot;

	/*
	/	this represents the default components the plugin will be using
	/	custom components can be set via blueprint
	*/
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Components)
		TSubclassOf<class UBaseRenderTargetComponent> RenderTargetComponent = UDefaultRenderTargetComponent::StaticClass();
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Components)
		TSubclassOf<class UBaseMovementAdjustmentComponent> AdjMovementComponent = UDefaultMovementAdjustmentComp::StaticClass();
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Components)
		TSubclassOf<class UBaseParticleSystemComponent> ParticleSystemComponent = UDefaultParticleSystemComponent::StaticClass();


private:
	UPROPERTY()
		ACharacter* Player;
	UPROPERTY()
		FFootPrintValues FootValues;
	UPROPERTY()
		UFoot* FootOnGround;
	UPROPERTY()
		USoundManager* SoundManager;
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
	void CreatePollutionFootPrint();
	void initComponents();
	void emittParticleEffect();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Constructor
	UFootPrintComponent();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create 2D Footprint", Keywords = "print plugin"), Category = "EasyFootPrints")
		void OnFootDown();
	UFUNCTION()
		UFoot* getFootOnGround() { return FootOnGround; };
	UFUNCTION()
		ACharacter* getPlayer() { return Player; };


};