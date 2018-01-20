// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MaterialInfo.h"
#include "Foot.generated.h"

/**
 * 
 */
struct FFootPrintValues;

UCLASS()
class UFoot : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
		FVector Location;

	UPROPERTY()
		FRotator Rotation;

	UPROPERTY()
		FName Name;

	UPROPERTY()
		FHitResult Hitresult = FHitResult(ForceInit);

	UPROPERTY()
		FLinearColor LastMaterialTouched;

	UPROPERTY()
		FLinearColor CurrentMaterial;

	UPROPERTY()
		float FootPollution = 0.0f;

	UPROPERTY()
		float CurrentPollutionScale = 0.0f;
	UPROPERTY()
		UMaterialInterface* HitMaterial;

	UFUNCTION()
		FLinearColor GetBaseColour();

	UFUNCTION()
		float GetPollutionScale();

	UFUNCTION()
		void AddPollution();


public:
	const FVector getLocation() { return Location; }
	FRotator getRotation() { return Rotation; }
	FName getBoneName() { return Name; }
	FHitResult* getHitresult() { return &Hitresult; }
	FLinearColor const getBaseColor() { return CurrentMaterial; }
	float const getFootPollution() { return FootPollution; }
	float getDepth();

	void setLocation(FVector loc) { Location = loc; }
	void setRotation(FFootPrintValues values);
	void setBoneName(FName name) { Name = name; }
	void setHitMaterial(UMaterialInterface* hitMaterial) { HitMaterial = hitMaterial; }
	bool const HasPollution() { return (FootPollution > 0.0f); }
	void IncreaseFootPollution();

	void DecreaseFootPollution();
};
