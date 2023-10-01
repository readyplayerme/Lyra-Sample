// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"

class FReadyPlayerMeAvatarCacheHandler
{
public:
	explicit FReadyPlayerMeAvatarCacheHandler(const FAvatarUri& AvatarUri);

	void SetUpdatedMetadataStr(const FString& MetadataJson, const FString& UpdatedDate);
	void SetModelData(const TArray<uint8>* Data);

	void SaveAvatarInCache() const;

	bool ShouldLoadFromCache() const;

	bool ShouldSaveMetadata() const;
	
	TOptional<FAvatarMetadata> GetLocalMetadata() const;

	static bool IsCachingEnabled();

private:
	bool IsMedataUpdated(const FString& UpdatedDate) const;
	
	const FAvatarUri AvatarUri;

	FString UpdatedMetadataStr;
	const TArray<uint8>* ModelData;
	bool bMetadataNeedsUpdate;
};
