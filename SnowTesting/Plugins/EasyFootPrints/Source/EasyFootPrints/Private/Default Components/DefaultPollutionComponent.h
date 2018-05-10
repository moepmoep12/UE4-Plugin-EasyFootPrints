// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base Abstract Components/BasePollutionComponent.h"
#include "DefaultPollutionComponent.generated.h"

/**
*  The default implementation for the BasePollutionComponent
*/
class UMaterialInterface;

UCLASS()
class UDefaultPollutionComponent : public UBasePollutionComponent
{
	GENERATED_BODY()

private:
	// The number of steps on the material
	UPROPERTY()
		int32 Steps;

	// the physical material of the current material
	UPROPERTY()
		UPhysMaterial_EasyFootPrints* CurrentPhysMat;


protected:
	// Adds pollution based on the amount of steps made
	void AddPollution();
	// sets pollution back to 0
	void resetPollution();
	// decreases pollution
	void DecreasePollution();

public:
	/** this is called when a foot touches a material with a pollutionfactor
	@PhysMat: the physcial material is used to get the PollutionFactor
	*/
	void increasePollution_Implementation(UPhysMaterial_EasyFootPrints* PhysMat) override;

	// returns true when pollution > 0
	bool  hasPollution_Implementation() const override;

	/** Spawns a DecalActor which is a pollution footprint
	@transform The transform for spawning the decal
	@Material: the material that will be used by the decal-actor
	@World: the current world to spawn in
	*/
	void createPollutionFootPrint_Implementation(FTransform transform, UMaterialInstanceDynamic* Material, UWorld* World) override;


};
