// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootPrintComponent.h"
#include "RenderTargetComputations.generated.h"

class UMaterialInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URenderTargetComputations : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URenderTargetComputations();

private:
	UPROPERTY()
		UFootPrintComponent* FootPrintComponent;
	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial = nullptr;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool hasHitComponentRenderTarget();
	void createFootPrintOnRenderTarget();
	FVector2D InitComputationOfRenderTargetScreenPosition();
	FVector2D ComputeRenderTargetScreenSize();
	FVector2D ComputeScreenPositionOnRenderTarget(FVector ActorLocation, FVector HitLocation, FVector ActorBounds);
	FVector2D Get2DVectorWithXAndYFrom3DVector(FVector VectorToBeComputed);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void initRenderTargetComputations(UFootPrintComponent* comp) { FootPrintComponent = comp; };
	bool computeRenderTarget();
	

		
	
};
