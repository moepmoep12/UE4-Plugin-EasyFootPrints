#pragma once

#include "CoreMinimal.h"
#include "RenderTargetValues.generated.h"

class UTextureRenderTarget2D;

/* This struct holds several values that are necessary for drawing onto a rendertarget*/

USTRUCT(BlueprintType)
struct FRenderTargetValues
{
	GENERATED_BODY()

		// the location of the hit actor
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		FVector2D ActorLocation;
	// the scale of the hit actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		FVector2D ActorScale;
	// the actor bounds of the hit actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		FVector2D ActorBounds;
	// the world location of the hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		FVector2D HitLocation;
	// the rotation of the foot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		float Rotation;
	// the rendertarget of the hit material to draw onto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
		UTextureRenderTarget2D* RenderTargetOfHitMaterial;
	// the density of the hit material, defined in its physical material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RenderTarget Values")
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
