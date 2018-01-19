// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MaterialInfo.generated.h"

/**
 * 
 */
UCLASS()
class UMaterialInfo : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
		UPhysicalMaterial* PhysMaterial;
	
	UPROPERTY()
		int32 AmountSteps;

public:
	UMaterialInfo();

	int32 getAmountSteps() { return AmountSteps; }
	UPhysicalMaterial* getPhysMaterial() { return PhysMaterial; }

	void incrementAmountSteps() {(AmountSteps<=10) ? AmountSteps++ : AmountSteps += 0 ; }
	void decrementAmountSteps() { (AmountSteps>=0) ? AmountSteps-- : AmountSteps +=0 ; }
	void setPhysMaterial(UPhysicalMaterial* physmat) { PhysMaterial = physmat; }
};
