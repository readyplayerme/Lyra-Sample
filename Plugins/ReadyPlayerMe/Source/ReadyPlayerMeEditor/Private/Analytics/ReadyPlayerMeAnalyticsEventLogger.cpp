// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeAnalyticsEventLogger.h"
#include "ReadyPlayerMeAmplitudeEventLogger.h"
#include "Dom/JsonObject.h"
#include "Kismet/KismetStringLibrary.h"

static const TMap<ERpmAnalyticsEventType, FString> EVENT_TO_STRING_MAP =
	{
		{ ERpmAnalyticsEventType::OpenProject, "open project"},
		{ ERpmAnalyticsEventType::CloseProject, "close project"},
		{ ERpmAnalyticsEventType::EnableCaching, "enable caching"},
		{ ERpmAnalyticsEventType::SetUserProperties, "set user properties"},
		{ ERpmAnalyticsEventType::AllowLogging, "allow tracking"},
		{ ERpmAnalyticsEventType::OpenIntegrationGuide, "open integration guide"},
		{ ERpmAnalyticsEventType::LoadQuickStartScene, "load quick start scene"},
		{ ERpmAnalyticsEventType::OpenAvatarDocumentation, "open avatar documentation"},
		{ ERpmAnalyticsEventType::OpenSamplesDocumentation, "open samples documentation"},
		{ ERpmAnalyticsEventType::OpenAnimationDocumentation, "open animation documentation"},
		{ ERpmAnalyticsEventType::OpenAvatarCreatorDocumentation, "open avatar creator documentation"},
		{ ERpmAnalyticsEventType::ViewPrivacyPolicy, "view privacy policy"}
	};

static const FString JSON_CACHING_ENABLED = "caching enabled";

FReadyPlayerMeAnalyticsEventLogger& FReadyPlayerMeAnalyticsEventLogger::Get()
{
	static FReadyPlayerMeAnalyticsEventLogger EventLogger;
	return EventLogger;
}

void FReadyPlayerMeAnalyticsEventLogger::LogProperties() const
{
	if (!bIsEnabled)
	{
		return;
	}
	Logger.LogEvent(EVENT_TO_STRING_MAP[ERpmAnalyticsEventType::SetUserProperties], Logger.MakeUserPropertiesJson());
}

void FReadyPlayerMeAnalyticsEventLogger::LogEvent(ERpmAnalyticsEventType EventType) const
{
	if (!bIsEnabled)
	{
		return;
	}
	Logger.LogEvent(EVENT_TO_STRING_MAP[EventType]);
}

void FReadyPlayerMeAnalyticsEventLogger::LogEnableAvatarCaching(bool bEnabled) const
{
	if (!bIsEnabled)
	{
		return;
	}
	const auto JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(JSON_CACHING_ENABLED, UKismetStringLibrary::Conv_BoolToString(bEnabled));
	Logger.LogEvent(EVENT_TO_STRING_MAP[ERpmAnalyticsEventType::EnableCaching], JsonObject);
}

void FReadyPlayerMeAnalyticsEventLogger::SetEnabled(bool bEnabled)
{
	bIsEnabled = bEnabled;
}

void FReadyPlayerMeAnalyticsEventLogger::SetAnalyticsTarget(const FString& Target)
{
	Logger.SetAnalyticsTarget(Target);
}

void FReadyPlayerMeAnalyticsEventLogger::EnableAnalytics()
{
	bIsEnabled = true;
	Logger.LogEvent(EVENT_TO_STRING_MAP[ERpmAnalyticsEventType::AllowLogging], Logger.MakeUserPropertiesJson());
}
