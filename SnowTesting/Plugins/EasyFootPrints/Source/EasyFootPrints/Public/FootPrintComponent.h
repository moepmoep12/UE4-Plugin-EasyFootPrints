// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Components/ActorComponent.h"
#include "FootValues.h"
#include "FootPrintComponent.generated.h"

/* Forward Declaration */
class UTextureRenderTarget2D;
class USoundManager;
class UCharacterMovementComponent;


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


private:
	UPROPERTY()
		ACharacter* Player;
	UPROPERTY()
		FFootPrintValues FootValues;
	UPROPERTY()
		UFoot* FootOnGround;
	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial = nullptr;
	UPROPERTY()
		UTexture* RenderTexture;
	UPROPERTY()
		USoundManager* SoundManager;
	UPROPERTY()
		float OriginaMaxWalkingSpeed;
	UPROPERTY()
		float CurrentMaxWalkSpeed;
	UPROPERTY()
		UCharacterMovementComponent* MovementComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void LoadFootPositions();
	void initFeet();
	void Trace();
	void setFootOnGround();
	void SpawnDecal();
	bool hasComponentRenderTarget();
	FVector2D InitComputationOfRenderTargetScreenPosition();
	FVector2D ComputeRenderTargetScreenSize();
	FVector2D ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds);
	FVector2D Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed);
	void CreatePollutionFootPrint();
	void resetWalkSpeed();
	void adjustMaxWalkSpeed(float depth);
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFootPrintComponent();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create 2D Footprint", Keywords = "print plugin"), Category = "EasyFootPrints")
		void OnFootDown();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "create FootPrint on RenderTarget", Keywords = "print plugin render create "), Category = "EasyFootPrints")
		void createFootPrintOnRenderTarget();


};
