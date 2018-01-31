// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "FootValues.h"
#include "BaseRenderTargetComponent.h"
#include "FootPrintComponent.generated.h"

/* Forward Declaration */
class UTextureRenderTarget2D;
class USoundManager;
class UCharacterMovementComponent;
class URenderTargetComputations;

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

	/*	custom components can be set via blueprint in the component section of FootPrintComponent
	/	they will change the behaviour of the plugin according to the implementation
	/	if null, the default component will be used
	*/
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Components)
		TSubclassOf<class UBaseRenderTargetComponent> RenderTargetComponent;
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = Components)
		TSubclassOf<class UBaseMovementAdjustmentComponent> AdjMovementComponent;


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
		UParticleSystem* CurrentFootprintParticleSystem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void LoadFootPositions();
	void initFeet();
	void Trace();
	void setFootOnGround();
	void CreatePollutionFootPrint();
	void EmittingParticleEffect(FVector Location);
	void EmittingParticleEffectWithPollution(FVector Location);
	void initComponents();

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