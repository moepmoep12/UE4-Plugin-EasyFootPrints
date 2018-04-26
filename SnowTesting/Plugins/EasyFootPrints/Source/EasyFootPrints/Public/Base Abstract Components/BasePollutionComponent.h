// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysMaterial_EasyFootPrints.h"
#include "BasePollutionComponent.generated.h"

class UMaterialInterface;

/** This is an abstract base class for a Pollution Component. 
*   Each foot has its own PollutionComponent which is responsible for creating PollutionFootPrints ( Decals )
*/

UCLASS( ClassGroup=(EasyFootPrints), abstract )
class EASYFOOTPRINTS_API UBasePollutionComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override {};

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override {};

	/** this is called when a foot touches a material with a pollutionfactor
		@PhysMat: the physcial material is used to get the PollutionFactor */
	virtual void increasePollution(UPhysMaterial_EasyFootPrints* PhysMat) {};

	/** Spawns a DecalActor which is a pollution footprint
		@transform The transform for spawning the decal
		@Material: the material that will be used by the decal-actor
		@World: the current world to spawn in */
	virtual void createPollutionFootPrint(FTransform transform, UMaterialInstanceDynamic* Material, UWorld* World) {};

	/** Returns true if the component has pollution > 0  */
	virtual bool const hasPollution() { return false; };
	
};
