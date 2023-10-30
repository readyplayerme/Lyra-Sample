// Copyright © 2021++ Ready Player Me


#include "AvatarCacheHandler.h"
#include "AvatarStorage.h"
#include "ReadyPlayerMeSettings.h"
#include "Utils/MetadataExtractor.h"


FAvatarCacheHandler::FAvatarCacheHandler(const FAvatarUri& AvatarUri)
	: AvatarUri(AvatarUri)
	, ModelData(nullptr)
	, bMetadataNeedsUpdate(false)
{
}

bool FAvatarCacheHandler::IsCachingEnabled()
{
	const UReadyPlayerMeSettings* Settings = GetDefault<UReadyPlayerMeSettings>();
	if (Settings)
	{
		return Settings->bEnableAvatarCaching;
	}
	return false;
}

bool FAvatarCacheHandler::ShouldLoadFromCache() const
{
	return IsCachingEnabled() && FAvatarStorage::AvatarExists(AvatarUri);
}

bool FAvatarCacheHandler::IsMedataUpdated(const FString& UpdatedDate) const
{
	const auto Metadata = GetLocalMetadata();
	if (Metadata.IsSet())
	{
		return Metadata->UpdatedAtDate != UpdatedDate;
	}
	return true;
}

TOptional<FAvatarMetadata> FAvatarCacheHandler::GetLocalMetadata() const
{
	const FString MetadataStr = FAvatarStorage::LoadMetadata(AvatarUri.LocalMetadataPath);
	if (!MetadataStr.IsEmpty())
	{
		return FMetadataExtractor::ExtractAvatarMetadata(MetadataStr);
	}
	return {};
}

bool FAvatarCacheHandler::ShouldSaveMetadata() const
{
	return bMetadataNeedsUpdate;
}

void FAvatarCacheHandler::SetUpdatedMetadataStr(const FString& MetadataJson, const FString& UpdatedDate)
{
	if (!IsCachingEnabled())
	{
		return;
	}
	bMetadataNeedsUpdate = IsMedataUpdated(UpdatedDate);
	UpdatedMetadataStr = MetadataJson;
	if (FAvatarStorage::FileExists(AvatarUri.LocalMetadataPath) && bMetadataNeedsUpdate)
	{
		FAvatarStorage::DeleteDirectory(AvatarUri.LocalAvatarDirectory);
	}
}

void FAvatarCacheHandler::SetModelData(const TArray<uint8>* Data)
{
	if (!IsCachingEnabled())
	{
		return;
	}
	// We store the pointer because we don't want to copy the avatar data.
	ModelData = Data;
}

void FAvatarCacheHandler::SaveAvatarInCache() const
{
	if (IsCachingEnabled() && ModelData != nullptr)
	{
		if (bMetadataNeedsUpdate)
		{
			FAvatarStorage::SaveMetadata(AvatarUri.LocalMetadataPath, UpdatedMetadataStr);
		}
		FAvatarStorage::SaveAvatar(AvatarUri.LocalModelPath, *ModelData);
	}
}
