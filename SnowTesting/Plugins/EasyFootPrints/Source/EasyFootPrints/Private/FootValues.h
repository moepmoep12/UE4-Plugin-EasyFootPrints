// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Foot.h"
#include "FootValues.generated.h"

class UFoot;

USTRUCT()
struct FFootPrintValues
{
	GENERATED_BODY()

	FVector ForwardVector;
	TArray<UFoot*> TrackedFeet;
	

	FVector getForwardVector() { return ForwardVector; }
	void setForwardVector(FVector forward) { ForwardVector = forward; }
	

	FFootPrintValues()
	{
		ForwardVector = FVector(0, 0, 0);
	}
};