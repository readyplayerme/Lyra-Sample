// Copyright © 2021++ Ready Player Me


#include "Utils/ReadyPlayerMeAvatarConfigProcessor.h"

#include "Utils/ReadyPlayerMeMorphTargetUtils.h"
#include "ReadyPlayerMePluginInfo.h"
#include "ReadyPlayerMeTypes.h"
#include "AvatarConfig/ReadyPlayerMeAvatarConfig.h"
#include "AvatarConfig/ReadyPlayerMeMorphTargetGroup.h"
#include "Kismet/KismetStringLibrary.h"

static const TMap<EAvatarPose, FString> POSE_TO_STRING =
{
	{ EAvatarPose::APose, "A" },
	{ EAvatarPose::TPose, "T" }
};

static const TMap<EAvatarTextureAtlas, FString> TEXTURE_ATLAS_TO_STRING =
{
	{ EAvatarTextureAtlas::None, "none" },
	{ EAvatarTextureAtlas::High, "1024" },
	{ EAvatarTextureAtlas::Medium, "512" },
	{ EAvatarTextureAtlas::Low, "256" }
};

static const TMap<EAvatarTextureChannel, FString> TEXTURE_CHANNEL_TO_STRING =
{
	{ EAvatarTextureChannel::BaseColor, "baseColor" },
	{ EAvatarTextureChannel::Normal, "normal" },
	{ EAvatarTextureChannel::MetallicRoughness, "metallicRoughness" },
	{ EAvatarTextureChannel::Emissive, "emissive" },
	{ EAvatarTextureChannel::Occlusion, "occlusion" }
};

static const TMap<EAvatarTextureSizeLimit, FString> TEXTURE_SIZE_LIMIT_TO_STRING =
{
	{ EAvatarTextureSizeLimit::Limit_1024, "1024" },
	{ EAvatarTextureSizeLimit::Limit_512, "512" },
	{ EAvatarTextureSizeLimit::Limit_256, "256" }
};

namespace
{
	FString ProcessTextureChannels(const TSet<EAvatarTextureChannel>& TextureChannels)
	{
		if (TextureChannels.Num() == 0)
		{
			return "none";
		}
		TArray<FString> ChannelStrList;
		for (const auto& Channel : TextureChannels)
		{
			ChannelStrList.Add(TEXTURE_CHANNEL_TO_STRING[Channel]);
		}
		return FString::Join(ChannelStrList, TEXT(","));
	}

	FString ProcessMorphTargets(const UReadyPlayerMeMorphTargetGroup* MorphTargetGroup)
	{
		if (MorphTargetGroup == nullptr)
		{
			return "";
		}
		TSet<EAvatarMorphTarget> Targets;
		TSet<EStandardMorphTargetGroup> Groups;
		MorphTargetGroup->GetTargets(Groups, Targets);
		if (Targets.Num() == 0 && Groups.Num() == 0)
		{
			return "";
		}
		TArray<FString> Morphs;
		for (const auto& Target : Targets)
		{
			Morphs.Add(FReadyPlayerMeMorphTargetUtils::MorphTargetToString(Target));
		}
		for (const auto& Group : Groups)
		{
			Morphs.Add(FReadyPlayerMeMorphTargetUtils::MorphTargetGroupToString(Group));
		}
		return "morphTargets=" + FString::Join(Morphs, TEXT(","));
	}
}

FString FReadyPlayerMeAvatarConfigProcessor::Process(UReadyPlayerMeAvatarConfig* AvatarConfig)
{
	if (!AvatarConfig)
	{
		return "";
	}
	const bool UseDraco = FReadyPlayerMePluginInfo::IsDracoPluginIncluded() && AvatarConfig->bUseDracoMeshCompression;
	const FString MorphTargetsParam = ProcessMorphTargets(AvatarConfig->MorphTargetGroup);
	TArray<FString> Parameters;
	Parameters.Add("pose=" + POSE_TO_STRING[AvatarConfig->Pose]);
	Parameters.Add("lod=" + FString::FromInt(static_cast<int>(AvatarConfig->Lod)));
	Parameters.Add("textureAtlas=" + TEXTURE_ATLAS_TO_STRING[AvatarConfig->TextureAtlas]);
	Parameters.Add("textureSizeLimit=" + TEXTURE_SIZE_LIMIT_TO_STRING[AvatarConfig->TextureSizeLimit]);
	Parameters.Add("textureChannels=" + ProcessTextureChannels(AvatarConfig->TextureChannels));
	if (!MorphTargetsParam.IsEmpty())
	{
		Parameters.Add(MorphTargetsParam);
	}
	Parameters.Add("useHands=" + UKismetStringLibrary::Conv_BoolToString(AvatarConfig->bUseHands));
	Parameters.Add("useMeshOptCompression=" + UKismetStringLibrary::Conv_BoolToString(AvatarConfig->bUseMeshOptCompression));
	Parameters.Add("useDracoMeshCompression=" + UKismetStringLibrary::Conv_BoolToString(UseDraco));
	return "?" + FString::Join(Parameters, TEXT("&"));
}
