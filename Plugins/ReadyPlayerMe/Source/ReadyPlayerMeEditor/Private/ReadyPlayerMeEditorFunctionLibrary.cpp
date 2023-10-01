// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeEditorFunctionLibrary.h"

#include "EditorUtilitySubsystem.h"
#include "Editor.h"
#include "ReadyPlayerMeEditorSettings.h"
#include "ReadyPlayerMeSettings.h"
#include "Analytics/ReadyPlayerMeAnalyticsEventLogger.h"
#include "Analytics/ReadyPlayerMeAnalyticsSetup.h"
#include "Runtime/Launch/Resources/Version.h"

void UReadyPlayerMeEditorFunctionLibrary::SetSubdomain(const FString& Subdomain)
{
	UReadyPlayerMeSettings* Settings = GetMutableDefault<UReadyPlayerMeSettings>();
	if (Settings)
	{
		Settings->Subdomain = Subdomain;
#if ENGINE_MAJOR_VERSION > 4
		Settings->TryUpdateDefaultConfigFile();
#else
		Settings->UpdateDefaultConfigFile();
#endif
		FReadyPlayerMeAnalyticsSetup::RemoveWidget();
	}
}

void UReadyPlayerMeEditorFunctionLibrary::EnableAnalytics()
{
	UReadyPlayerMeEditorSettings* Settings = GetMutableDefault<UReadyPlayerMeEditorSettings>();
	if (Settings)
	{
		Settings->bEnableAnalytics = true;
#if ENGINE_MAJOR_VERSION > 4
		Settings->TryUpdateDefaultConfigFile();
#else
		Settings->UpdateDefaultConfigFile();
#endif
		FReadyPlayerMeAnalyticsEventLogger::Get().EnableAnalytics();
		FReadyPlayerMeAnalyticsSetup::RemoveWidget();
	}
}

FString UReadyPlayerMeEditorFunctionLibrary::GetSubdomain()
{
	const UReadyPlayerMeSettings* Settings = GetDefault<UReadyPlayerMeSettings>();
	if (Settings)
	{
		return Settings->Subdomain;
	}
	return {};
}

void UReadyPlayerMeEditorFunctionLibrary::LogRpmEvent(ERpmAnalyticsEventType EventType)
{
	FReadyPlayerMeAnalyticsEventLogger::Get().LogEvent(EventType);
}

void UReadyPlayerMeEditorFunctionLibrary::CloseEditorWidget(const FString& Name)
{
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (IsValid(EditorUtilitySubsystem))
	{
		auto Found = EditorUtilitySubsystem->RegisteredTabs.FilterByPredicate([Name](const auto& Pair){ return Pair.Key.ToString().Contains(Name);});
		if (Found.Num() != 0)
		{
			EditorUtilitySubsystem->CloseTabByID(Found.begin()->Key);
		}
	}
}
