// Fill out your copyright notice in the Description page of Project Settings.

#include "Foot.h"
#include "FootValues.h"
#include "Engine/GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"



void UFoot::setRotation(FFootPrintValues values)
{
	FQuat floorRot = FRotationMatrix::MakeFromZX(Hitresult.Normal, values.getForwardVector()).ToQuat();
	FQuat offsetRot(FRotator(0.0f, 90.0f, 0.0f));
	Rotation = (floorRot * offsetRot).Rotator();
}

void UFoot::IncreaseFootPollution()
{
	if (!Hitresult.GetComponent()) {
		return;
	}

	if (GetBaseColour() != CurrentMaterial) {
		CurrentMaterial = GetBaseColour();
		CurrentPollutionScale = GetPollutionScale();
	}

	AddPollution();


}

void UFoot::DecreaseFootPollution()
{
	float Subtractor = 1 - FMath::Exp(-CurrentPollutionScale / 5);
	FootPollution = FMath::Clamp<float>(FootPollution - Subtractor, 0.0f, 1.0f);

	if (FootPollution <= 0.0f) 
	{
		CurrentPollutionScale = 0.0f;
		CurrentMaterial = FLinearColor(0, 0, 0, 0);
	}
}

FLinearColor UFoot::GetBaseColour()
{
	TArray<UMaterialInterface*> MaterialsOfHitComponent;
	FLinearColor BaseColor;
	Hitresult.GetComponent()->GetUsedMaterials(MaterialsOfHitComponent);

	for (UMaterialInterface* MaterialOfComponent : MaterialsOfHitComponent)
	{
		if (MaterialOfComponent->GetVectorParameterValue(FName("BaseTexture"), BaseColor))
			return BaseColor;
	}
	return FLinearColor(0, 0, 0);
}

float UFoot::GetPollutionScale()
{
	TArray<UMaterialInterface*> MaterialsOfHitComponent;
	float PollutionScale = 0;
	Hitresult.GetComponent()->GetUsedMaterials(MaterialsOfHitComponent);
	for (UMaterialInterface* MaterialOfComponent : MaterialsOfHitComponent)
	{
		if (MaterialOfComponent->GetScalarParameterValue(FName("PollutionScale"), PollutionScale))
			return PollutionScale;
	}
	return 0.0f;
}

void UFoot::AddPollution()
{
	float PollutionScale = GetPollutionScale();
	FootPollution = FMath::Clamp<float>((1 - FMath::Exp(-PollutionScale / 5) + FootPollution), 0.0f, 1.0f);
}


/*
bool UFoot::ChangeMaterial(UPhysicalMaterial* physmat)
{

	if (!CurrentMaterial) {
		CurrentMaterial = NewObject<UMaterialInfo>();
		CurrentMaterial->setPhysMaterial(physmat);
	}

	if (CurrentMaterial->getPhysMaterial() == physmat)
	{
		if (LastMaterialTouched && LastMaterialTouched->getAmountSteps() > 0)
		{
			LastMaterialTouched->decrementAmountSteps();
			return true;
		}
			CurrentMaterial->incrementAmountSteps();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("Current == Last"));
			return false;
	}

	else
	{
		LastMaterialTouched = CurrentMaterial;
		CurrentMaterial = NewObject<UMaterialInfo>();
		CurrentMaterial->setPhysMaterial(physmat);
		LastMaterialTouched->decrementAmountSteps();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString("Current != Last"));
		return true;
	}
}
*/