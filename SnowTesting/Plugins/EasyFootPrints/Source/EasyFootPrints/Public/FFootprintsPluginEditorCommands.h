// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
#include "EditorStyleSet.h"
#include "CoreMinimal.h"

/**
 * 
 */
class FFootprintsPluginEditorCommands: public TCommands<FFootprintsPluginEditorCommands>
{
public:

	FFootprintsPluginEditorCommands()
		: TCommands<FFootprintsPluginEditorCommands>
		(
			TEXT("FootprintsPluginEditor"),
			NSLOCTEXT("Contexts", "FootprintsPluginEditor", "FootprintsPluginEditor Plugin"),
			NAME_None,
			FEditorStyle::GetStyleSetName()
			) {}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> OpensWiki;
};
