// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BasePollutionComponent.h"
#include "DefaultPollutionComponent.generated.h"

/**
 * 
 */
class UMaterialInterface;

UCLASS()
class UDefaultPollutionComponent : public UBasePollutionComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		int32 Steps;
	UPROPERTY()
		UPhysMaterial_EasyFootPrints* CurrentPhysMat;
	UPROPERTY(meta = (ClampMin = "0.0", ClampMax = "1.0"))
		float Pollution = 0.0f;

protected:
	void AddPollution();
	void resetPollution();
	void DecreasePollution();

public:
	void increasePollution(UPhysMaterial_EasyFootPrints* PhysMat) override;
	bool const hasPollution() override;
	void createPollutionFootPrint(FVector Location,FRotator Rotation, UMaterialInterface* Material, UWorld* World) override;
	
	
};
