#pragma once

#include "CoreMinimal.h"
#include "RenderTargetValues.generated.h"

class UTextureRenderTarget2D;

USTRUCT(BlueprintType)
struct FRenderTargetValues
{
	GENERATED_BODY()

	// The location of the hit actor
	UPROPERTY()
		FVector2D ActorLocation;

	// the x-y scale of the hit actor
	UPROPERTY()
		FVector2D ActorScale;

	// the actor bounds of the hit actor
	UPROPERTY()
		FVector2D ActorBounds;

	// the world location of the hit
	UPROPERTY()
		FVector2D HitLocation;

	UPROPERTY()
		float Rotation;

	UPROPERTY()
		UTextureRenderTarget2D* RenderTargetOfHitMaterial;

	UPROPERTY()
		int32 Density;

	FRenderTargetValues() {
		RenderTargetOfHitMaterial = nullptr;
	}


	void clear() {
		this->ActorLocation = FVector2D(0, 0);
		this->ActorScale = FVector2D(0, 0);
		this->ActorBounds = FVector2D(0, 0);
		this->HitLocation = FVector2D(0, 0);
		this->Rotation = 0;
		this->Density = 0;
		this->RenderTargetOfHitMaterial = nullptr;
	}

};
