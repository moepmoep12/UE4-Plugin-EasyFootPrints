// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPollutionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInterface.h"
#include "FootPrintDecal.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"

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

void UDefaultPollutionComponent::AddPollution()
{
	Steps++;
	float PollutionFactor = CurrentPhysMat->PollutionFactor;
	float NewPollution = (1 / PollutionFactor) * UKismetMathLibrary::Loge(Steps* PollutionFactor);
	Pollution = UKismetMathLibrary::FClamp(NewPollution, 0.0, 1.0);
}

void UDefaultPollutionComponent::resetPollution()
{
	Steps = 0;
	Pollution = 0.0f;
}

bool const UDefaultPollutionComponent::hasPollution()
{
	return Pollution > 0.0f;
}

void UDefaultPollutionComponent::createPollutionFootPrint(FVector Location,FRotator Rotation, UMaterialInterface * Material, UWorld* World)
{
	if (!World) {
		return;
	}
	ADecalActor* Adecal = World->SpawnActor<ADecalActor>(AFootPrintDecal::StaticClass(),Location, Rotation);
	
	Adecal->SetDecalMaterial(Material);
	UMaterialInstanceDynamic* matinstance = Adecal->CreateDynamicMaterialInstance();
	Adecal->SetDecalMaterial(matinstance);
	FLinearColor ColorA = CurrentPhysMat->PollutionColor;
	ColorA.A = Pollution;

	matinstance->SetVectorParameterValue(FName("FootPrintColor"), ColorA);

	DecreasePollution();
}

void UDefaultPollutionComponent::DecreasePollution()
{
	Steps--;
	float PollutionFactor = CurrentPhysMat->PollutionFactor;
	float NewPollution = (1 / PollutionFactor) * UKismetMathLibrary::Loge(Steps* PollutionFactor);
	Pollution = UKismetMathLibrary::FClamp(NewPollution, 0.0, 1.0);
}

