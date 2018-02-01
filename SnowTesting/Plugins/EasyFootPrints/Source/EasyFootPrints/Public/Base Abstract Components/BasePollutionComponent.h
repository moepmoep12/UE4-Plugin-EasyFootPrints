// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysMaterial_EasyFootPrints.h"
#include "BasePollutionComponent.generated.h"

class UMaterialInterface;

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

	/** this is called when a foot touches a material with pollution
		*@PhysMat: the physcial material is used to get the PollutionScale */
	virtual void increasePollution(UPhysMaterial_EasyFootPrints* PhysMat) {};

	/** Spawns a DecalActor which is a pollution footprint
		@Location: the location for spawning
		@Rotation: the rotation for spawning
		@Material: the material that will be used by the decalactor
		@World: the current world to spawn in */
	virtual void createPollutionFootPrint(FVector Location,FRotator Rotation, UMaterialInterface* Material, UWorld* World) {};

	/** Returns whether the foot this component belongs to has any pollution */
	virtual bool const hasPollution() { return false; };
	
};
