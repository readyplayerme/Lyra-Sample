// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeAmplitudeEventLogger.h"
#include "ReadyPlayerMeEditorTypes.h"

class FReadyPlayerMeAnalyticsEventLogger
{
public:
	static FReadyPlayerMeAnalyticsEventLogger& Get();

	void LogProperties() const;
	void LogEvent(ERpmAnalyticsEventType EventType) const;
	void LogEnableAvatarCaching(bool bEnabled) const;
	void SetEnabled(bool bEnabled);
	void SetAnalyticsTarget(const FString& Target);
	void EnableAnalytics();

private:
	FReadyPlayerMeAmplitudeEventLogger Logger;
	bool bIsEnabled = false;
};
