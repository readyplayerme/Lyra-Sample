// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeEditorSettings.h"
#include "Analytics/ReadyPlayerMeAnalyticsSetup.h"
#include "Analytics/ReadyPlayerMeAnalyticsEventLogger.h"

UReadyPlayerMeEditorSettings::UReadyPlayerMeEditorSettings()
	: bEnableAnalytics(false)
{
}

#if WITH_EDITOR
void UReadyPlayerMeEditorSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (bEnableAnalytics)
	{
		FReadyPlayerMeAnalyticsEventLogger::Get().EnableAnalytics();
		FReadyPlayerMeAnalyticsSetup::RemoveWidget();
	}
	else
	{
		FReadyPlayerMeAnalyticsEventLogger::Get().SetEnabled(false);
	}
}
#endif
