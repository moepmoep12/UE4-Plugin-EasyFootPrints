// Fill out your copyright notice in the Description page of Project Settings.

#include "Foot.h"
#include "FootValues.h"
#include "Engine/GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "LandscapeComponent.h"



void UFoot::setRotation(FFootPrintValues values)
{
	FQuat floorRot = FRotationMatrix::MakeFromZX(Hitresult.Normal, values.getForwardVector()).ToQuat();
	FQuat offsetRot(FRotator(0.0f, 90.0f, 0.0f));
	Rotation = (floorRot * offsetRot).Rotator();
}

void UFoot::IncreaseFootPollution()
{
	if (!HitMaterial) {
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
	FLinearColor BaseColor;

	if (HitMaterial->GetVectorParameterValue(FName("BaseTexture"), BaseColor)) {
		return BaseColor;
	}

	return FLinearColor(0, 0, 0);
}

float UFoot::GetPollutionScale()
{
	float PollutionScale = 0;

	if (HitMaterial->GetScalarParameterValue(FName("PollutionScale"), PollutionScale)) {
		return PollutionScale;
	}
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Green, FString("PollutionScale = ") + FString::SanitizeFloat(PollutionScale));
	return 0.0f;
}

void UFoot::AddPollution()
{
	float PollutionScale = GetPollutionScale();
	FootPollution = FMath::Clamp<float>((1 - FMath::Exp(-PollutionScale / 5) + FootPollution), 0.0f, 1.0f);
}

float UFoot::getDepth()
{
	float depth = 0;
	HitMaterial->GetScalarParameterValue(FName("Depth"), depth);
	return depth; 
}

float UFoot::getTessellationHeight() {

	float tessellationHeight = 0;
	HitMaterial->GetScalarParameterValue(FName("TessellationHeight"), tessellationHeight);
	return tessellationHeight;
}