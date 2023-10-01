// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeEditor.h"

#include "Analytics/ReadyPlayerMeAnalyticsSetup.h"
#include "Analytics/ReadyPlayerMeAnalyticsEventLogger.h"
#include "Settings/ProjectPackagingSettings.h"
#include "Runtime/Launch/Resources/Version.h"

#define LOCTEXT_NAMESPACE "FReadyPlayerMeEditorModule"

namespace
{
	void AddGLTFRuntimeToCookingDirectories()
	{
		UProjectPackagingSettings* PackagingSetting = GetMutableDefault<UProjectPackagingSettings>();
		if (!PackagingSetting->DirectoriesToAlwaysCook.ContainsByPredicate([](const auto& Item){ return Item.Path == "/glTFRuntime";} ))
		{
			PackagingSetting->DirectoriesToAlwaysCook.Add(FDirectoryPath{"/glTFRuntime"});
#if ENGINE_MAJOR_VERSION > 4
			PackagingSetting->TryUpdateDefaultConfigFile();
#else
			PackagingSetting->UpdateDefaultConfigFile();
#endif
		}
	}
}

void FReadyPlayerMeEditorModule::StartupModule()
{
	AddGLTFRuntimeToCookingDirectories();
	FReadyPlayerMeAnalyticsSetup::Startup();
	FReadyPlayerMeAnalyticsEventLogger::Get().LogProperties();
	FReadyPlayerMeAnalyticsEventLogger::Get().LogEvent(ERpmAnalyticsEventType::OpenProject);
}

void FReadyPlayerMeEditorModule::ShutdownModule()
{
	FReadyPlayerMeAnalyticsEventLogger::Get().LogEvent(ERpmAnalyticsEventType::CloseProject);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FReadyPlayerMeEditorModule, ReadyPlayerMeEditor)
