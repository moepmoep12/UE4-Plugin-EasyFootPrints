// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RenderTargetValues.h"
#include "BaseRenderTargetComponent.generated.h"

class UFootPrintComponent;


UCLASS(ClassGroup = (EasyFootPrints), meta = (IsBlueprintBase = "true"), abstract)
class EASYFOOTPRINTS_API UBaseRenderTargetComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override { Super::BeginPlay(); };

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};

	/* draws the material onto the render target
	@MaterialToDraw: the material that is used for drawing
	@RenderTargetValues: a struct with basic information such as Location and the RenderTarget
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Draw on RenderTarget", Keywords = "render target footprint plugin"), Category = "EasyFootPrints")
		void drawOnRenderTarget(UMaterialInterface * FootPrintShape, struct FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform);

	virtual void drawOnRenderTarget_Implementation(UMaterialInterface * FootPrintShape, struct FRenderTargetValues& RenderTargetValues, FTransform ShapeTransform) {};




};
