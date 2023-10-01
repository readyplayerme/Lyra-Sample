// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"
#include "glTFRuntimeParser.h"
#include "ReadyPlayerMeMemoryCache.generated.h"

USTRUCT(BlueprintType)
struct FAvatarPreloadData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FString Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	class UReadyPlayerMeAvatarConfig* AvatarConfig = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	USkeleton* TargetSkeleton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;
};

USTRUCT(BlueprintType)
struct FAvatarMemoryCacheData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FString Url;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FString AvatarConfigHash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
	FAvatarMetadata Metadata;
};

/**
 * Is used to preload avatars and store the cached skeletal meshes.
 * Cached avatars will be instantiated instantly. 
 */
UCLASS(Blueprintable, BlueprintType)
class READYPLAYERME_API UReadyPlayerMeMemoryCache : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Preloads the avatar data so the avatars would be instantiated instantly when loading them from the ReadyPlayerMeComponent.
	 * 
	 * @param PreloadDataList List of avatar data needed for preloading.
	 * @param OnPreloadCompleted Complete callback. Called when the avatars are preloaded.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Preload", AutoCreateRefTerm = "OnPreloadCompleted"))
	void Preload(const TArray<FAvatarPreloadData>& PreloadDataList, const FAvatarPreloadCompleted& OnPreloadCompleted);

	/**
	 * Returns the preload avatar data for a specific avatar.
	 *
	 * @param Url Avatar url.
	 * @param AvatarConfig Avatar config.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	FAvatarMemoryCacheData GetAvatarCacheData(const FString& Url, UReadyPlayerMeAvatarConfig* AvatarConfig) const;

	/**
	 * Adds an already loaded avatar to the memory cache.
	 *
	 * @param Url Avatar url.
	 * @param AvatarConfig Avatar config.
	 * @param SkeletalMesh Preloaded skeletalMesh of the avatar.
	 * @param Metadata Preloaded avatar Metadata.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void AddAvatar(const FString& Url, UReadyPlayerMeAvatarConfig* AvatarConfig, USkeletalMesh* SkeletalMesh, const FAvatarMetadata& Metadata);

	/**
	 * Removes specific avatar data from the memory cache.
	 *
	 * @param Url of the avatar.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void RemoveAvatar(const FString& Url);

	/** Clears all avatars from the memory cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me")
	void ClearAvatars();

	/** Avatar Data for all the preloaded avatars. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReadyPlayerMe")
	TArray<FAvatarMemoryCacheData> CachedAvatars;

private:
	UFUNCTION()
	void OnAvatarDownloaded(USkeletalMesh* SkeletalMesh, const FAvatarMetadata& Metadata);

	UFUNCTION()
	void OnAvatarLoadFailed(const FString& ErrorMessage);

	void CompleteLoading();

	UPROPERTY()
	TMap<class UReadyPlayerMeAvatarLoader*, FAvatarPreloadData> AvatarLoaders;

	FAvatarDownloadCompleted OnAvatarDownloadCompleted;

	FAvatarLoadFailed OnLoadFailed;

	FAvatarPreloadCompleted OnAvatarPreloadCompleted;

	int32 FailedRequestCount = 0;
};
