// Fill out your copyright notice in the Description page of Project Settings.

#include "FootPrintDecal.h"
#include "Components/DecalComponent.h"

/* Decalsize is set on the specified value
*/
AFootPrintDecal::AFootPrintDecal()
{
	GetDecal()->DecalSize = FVector(20, 30, 20);
}

