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

	virtual void increasePollution(UPhysMaterial_EasyFootPrints* PhysMat) {};
	virtual void createPollutionFootPrint(FVector Location,FRotator Rotation, UMaterialInterface* Material, UWorld* World) {};
	virtual bool const hasPollution() { return false; };
	
};
