// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeAnalyticsData.h"

#include "ReadyPlayerMeSettings.h"
#include "ReadyPlayerMe/Private/Utils/ReadyPlayerMePluginInfo.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/EngineVersion.h"

static const FString SDK_TARGET = "Unreal";

namespace
{

	FString GetDeviceId()
	{
		const TArray<uint8> MacAddress = FPlatformMisc::GetMacAddress();
		return FMD5::HashBytes(MacAddress.GetData(), MacAddress.Num());
	}

	FString GetAppId()
	{
		const UReadyPlayerMeSettings* Settings = GetDefault<UReadyPlayerMeSettings>();
		if (IsValid(Settings))
		{
			return Settings->AppId;
		}
		return {};
	}

	FString GetSubdomain()
	{
		const UReadyPlayerMeSettings* Settings = GetDefault<UReadyPlayerMeSettings>();
		if (IsValid(Settings))
		{
			return Settings->Subdomain;
		}
		return {};
	}
}

FReadyPlayerMeAnalyticsData::FReadyPlayerMeAnalyticsData()
{
	const auto EngineVersion = FEngineVersion::Current();
	AppName = FApp::GetProjectName();
	SDKVersion = FReadyPlayerMePluginInfo::GetRpmPluginVersion();
	AvatarCreatorVersion = FReadyPlayerMePluginInfo::GetAvatarCreatorPluginVersion();
	SdkTarget = SDK_TARGET;
	Subdomain = GetSubdomain();
	AppId = GetAppId();
	UnrealVersion = FString::Printf(TEXT("%d.%d.%d"), EngineVersion.GetMajor(), EngineVersion.GetMinor(), EngineVersion.GetPatch());
	Platform = UGameplayStatics::GetPlatformName();
	DeviceId = GetDeviceId();
	AppIdentifier = UKismetSystemLibrary::GetGameBundleId();
	bIsCppPlugin = FReadyPlayerMePluginInfo::IsCppPlugin();
}
