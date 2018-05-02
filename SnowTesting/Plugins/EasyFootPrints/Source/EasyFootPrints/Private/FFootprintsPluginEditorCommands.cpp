// Fill out your copyright notice in the Description page of Project Settings.

#include "FFootprintsPluginEditorCommands.h"
#include "EasyFootPrints.h"

#define LOCTEXT_NAMESPACE "FFootprintsPluginEditorModule"

void FFootprintsPluginEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpensWiki, "How to", "If you have problems with using the Footprints Plugin, click here and you will find answers to most of your questions in the Plugin Wiki", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
