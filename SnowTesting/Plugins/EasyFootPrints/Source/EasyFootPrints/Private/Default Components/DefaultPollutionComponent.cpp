// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPollutionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInterface.h"
#include "FootPrintDecal.h"
#include "Engine/World.h"
#include "TextureResource.h"
#include "Materials/MaterialInstanceDynamic.h"


/** this is called when a foot touches a material with a pollutionfactor
	@PhysMat: Used for getting the PollutionFactor, aswell as adding Pollution if Physmat hasn't changed since the last step
*/
void UDefaultPollutionComponent::increasePollution(UPhysMaterial_EasyFootPrints* PhysMat)
{
	if (!CurrentPhysMat) {
		CurrentPhysMat = PhysMat;
	}

	if (CurrentPhysMat != PhysMat) {
		CurrentPhysMat = PhysMat;
		resetPollution();
	}

	AddPollution();


}

/** Increases steps in the current material and calculating the new Pollution */
void UDefaultPollutionComponent::AddPollution()
{
	Steps++;
	float PollutionFactor = CurrentPhysMat->PollutionFactor;
	float NewPollution = (1 / PollutionFactor) * UKismetMathLibrary::Loge(Steps* PollutionFactor);
	Pollution = UKismetMathLibrary::FClamp(NewPollution, 0.0, 1.0);
}

/* Resets steps and Pollution to 0 */
void UDefaultPollutionComponent::resetPollution()
{
	Steps = 0;
	Pollution = 0.0f;
}

bool const UDefaultPollutionComponent::hasPollution()
{
	return Pollution > 0.0f;
}

/** Spawns a DecalActor which is a pollution footprint
	@transform The transform for spawning the decal
	@Material: the material that will be used by the decal-actor
	@World: the current world to spawn in
*/
void UDefaultPollutionComponent::createPollutionFootPrint(FTransform transform, UMaterialInstanceDynamic * Material, UWorld* World)
{
	if (!World) {
		return;
	}
	ADecalActor* Adecal = World->SpawnActor<ADecalActor>(AFootPrintDecal::StaticClass(),transform);
	Adecal->SetDecalMaterial(Material);
	//UMaterialInstanceDynamic* matinstance = Adecal->CreateDynamicMaterialInstance();
	//Adecal->SetDecalMaterial(matinstance);
	FLinearColor ColorA = CurrentPhysMat->PollutionColor;
	ColorA.A = Pollution;

	Material->SetVectorParameterValue(FName("FootPrintColor"), ColorA);

	DecreasePollution();
}

void UDefaultPollutionComponent::DecreasePollution()
{
	Steps--;
	float PollutionFactor = CurrentPhysMat->PollutionFactor;
	float NewPollution = (1 / PollutionFactor) * UKismetMathLibrary::Loge(Steps* PollutionFactor);
	Pollution = UKismetMathLibrary::FClamp(NewPollution, 0.0, 1.0);
}

