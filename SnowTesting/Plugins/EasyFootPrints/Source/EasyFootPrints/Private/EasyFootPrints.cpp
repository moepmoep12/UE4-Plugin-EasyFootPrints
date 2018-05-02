// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "EasyFootPrints.h"
#include "FFootprintsPluginEditorCommands.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"

#define LOCTEXT_NAMESPACE "FEasyFootPrintsModule"

void FEasyFootPrintsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FFootprintsPluginEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FFootprintsPluginEditorCommands::Get().OpensWiki,
		FExecuteAction::CreateRaw(this, &FEasyFootPrintsModule::OpenTheWiki)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	
		TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
		/**
		* Extends a tool bar at the specified extension point
		*
		* @param	ExtensionHook			Part of the menu to extend.  You can extend the same point multiple times, and extensions will be applied in the order they were registered.
		* @param	HookPosition			Where to apply hooks in relation to the extension hook
		* @param	CommandList				The UI command list responsible for handling actions for the toolbar items you'll be extending the menu with
		* @param	ToolbarExtensionDelegate	Called to populate the part of the toolbar you're extending
		*
		* @return	Pointer to the new extension object.  You can use this later to remove the extension.
		*/
		NewToolbarExtender->AddToolBarExtension("Content",
			EExtensionHook::Before,
			PluginCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FEasyFootPrintsModule::AddToolbarButton));
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);
	
}

void FEasyFootPrintsModule::OpenTheWiki()
{
	UE_LOG(LogTemp, Warning, TEXT("Opens Wiki of the Footprints Plugin!"));
	FPlatformProcess::LaunchURL(TEXT("https://gitlab2.informatik.uni-wuerzburg.de/GE/Teaching/2017WS-2018SS/GL2/01-pohli-schmidt/wikis/how-to-setup"), NULL, NULL);
}

void FEasyFootPrintsModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FFootprintsPluginEditorCommands::Get().OpensWiki);
}

void FEasyFootPrintsModule::ShutdownModule()
{
	
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyFootPrintsModule, EasyFootPrints)