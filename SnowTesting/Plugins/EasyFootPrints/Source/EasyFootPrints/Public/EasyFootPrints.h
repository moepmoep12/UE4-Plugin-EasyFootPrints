// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FFootprintsPluginEditorCommands.h"
#include "ModuleManager.h"

class FEasyFootPrintsModule : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/*
	TSharedPtr<FUICommandList> PluginCommands;
	void OpenTheWiki();
	void AddToolbarButton(FToolBarBuilder& Builder);
	*/
};