// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeMemoryCache.h"
#include "ReadyPlayerMeAvatarLoader.h"
#include "Utils/ReadyPlayerMeAvatarConfigProcessor.h"

namespace
{
	FString MakeAvatarConfigHash(UReadyPlayerMeAvatarConfig* AvatarConfig)
	{
		const FString UrlQueryString = FReadyPlayerMeAvatarConfigProcessor::Process(AvatarConfig);
		return UrlQueryString.IsEmpty() ? "" : FMD5::HashAnsiString(*UrlQueryString).Left(8);
	}
}

void UReadyPlayerMeMemoryCache::Preload(const TArray<FAvatarPreloadData>& PreloadDataList, const FAvatarPreloadCompleted& OnPreloadCompleted)
{
	OnAvatarDownloadCompleted.BindDynamic(this, &UReadyPlayerMeMemoryCache::OnAvatarDownloaded);
	OnLoadFailed.BindDynamic(this, &UReadyPlayerMeMemoryCache::OnAvatarLoadFailed);
	OnAvatarPreloadCompleted = OnPreloadCompleted;

	for (const auto& PreloadData : PreloadDataList)
	{
		const FAvatarMemoryCacheData CacheData = GetAvatarCacheData(PreloadData.Url, PreloadData.AvatarConfig);
		if (CacheData.SkeletalMesh == nullptr)
		{
			UReadyPlayerMeAvatarLoader* AvatarLoader = NewObject<UReadyPlayerMeAvatarLoader>(this);
			AvatarLoader->LoadAvatar(PreloadData.Url, PreloadData.AvatarConfig, PreloadData.TargetSkeleton,
				PreloadData.SkeletalMeshConfig, OnAvatarDownloadCompleted, OnLoadFailed);
			AvatarLoaders.Add(AvatarLoader, PreloadData);
		}
	}
}

FAvatarMemoryCacheData UReadyPlayerMeMemoryCache::GetAvatarCacheData(const FString& Url, UReadyPlayerMeAvatarConfig* AvatarConfig) const
{
	const FString ConfigHash = MakeAvatarConfigHash(AvatarConfig);
	const auto CacheData = CachedAvatars.FindByPredicate([&Url, &ConfigHash](const FAvatarMemoryCacheData& Data){return Data.Url == Url && Data.AvatarConfigHash == ConfigHash;});
	if (CacheData != nullptr)
	{
		return *CacheData;
	}
	return {};
}

void UReadyPlayerMeMemoryCache::AddAvatar(const FString& Url, UReadyPlayerMeAvatarConfig* AvatarConfig, USkeletalMesh* SkeletalMesh, const FAvatarMetadata& Metadata)
{
	const FString ConfigHash = MakeAvatarConfigHash(AvatarConfig);
	const FAvatarMemoryCacheData CacheData = GetAvatarCacheData(Url, AvatarConfig);
	if (CacheData.SkeletalMesh == nullptr)
	{
		CachedAvatars.Add({Url, ConfigHash, SkeletalMesh, Metadata});
	}
}

void UReadyPlayerMeMemoryCache::RemoveAvatar(const FString& Url)
{
	CachedAvatars.RemoveAll([&Url](const FAvatarMemoryCacheData& Data){return Data.Url == Url;});
}

void UReadyPlayerMeMemoryCache::ClearAvatars()
{
	CachedAvatars.Empty();
}

void UReadyPlayerMeMemoryCache::OnAvatarDownloaded(USkeletalMesh* SkeletalMesh, const FAvatarMetadata& Metadata)
{
	const UReadyPlayerMeAvatarLoader* AvatarLoader = nullptr;
	// TODO: Refactor this after switching to 4.27
	for (const auto& Pair : AvatarLoaders)
	{
		if (Pair.Key->SkeletalMesh == SkeletalMesh)
		{
			AvatarLoader = Pair.Key;
			break;
		}
	}

	AddAvatar(AvatarLoaders[AvatarLoader].Url, AvatarLoaders[AvatarLoader].AvatarConfig, SkeletalMesh, Metadata);
	AvatarLoaders.Remove(AvatarLoader);
	CompleteLoading();
}

void UReadyPlayerMeMemoryCache::OnAvatarLoadFailed(const FString& ErrorMessage)
{
	++FailedRequestCount;
	CompleteLoading();
}

void UReadyPlayerMeMemoryCache::CompleteLoading()
{
	if (AvatarLoaders.Num() == FailedRequestCount)
	{
		OnLoadFailed.Unbind();
		OnAvatarDownloadCompleted.Unbind();
		AvatarLoaders.Empty();
		(void)OnAvatarPreloadCompleted.ExecuteIfBound(FailedRequestCount == 0);
	}
}
