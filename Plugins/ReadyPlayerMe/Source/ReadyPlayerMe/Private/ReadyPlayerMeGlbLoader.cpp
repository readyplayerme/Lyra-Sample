// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeGlbLoader.h"
#include "Utils/ReadyPlayerMeGlTFConfigCreator.h"
#include "Utils/ReadyPlayerMeMetadataExtractor.h"
#include "glTFRuntimeFunctionLibrary.h"

UReadyPlayerMeGlbLoader::UReadyPlayerMeGlbLoader()
{
	OnSkeletalMeshCallback.BindDynamic(this, &UReadyPlayerMeGlbLoader::OnSkeletalMeshLoaded);
}
                                              
void UReadyPlayerMeGlbLoader::LoadFromFile(const FString& LocalModelPath, EAvatarBodyType BodyType, const FGlbLoadCompleted& LoadCompleted)
{
	OnLoadCompleted = LoadCompleted;
	UglTFRuntimeAsset* Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(LocalModelPath, false, FReadyPlayerMeGlTFConfigCreator::GetGlTFRuntimeConfig());
	LoadSkeletalMesh(Asset, BodyType);
}

void UReadyPlayerMeGlbLoader::LoadFromData(const TArray<uint8>& Data, EAvatarBodyType BodyType, const FGlbLoadCompleted& LoadCompleted)
{
	OnLoadCompleted = LoadCompleted;
	UglTFRuntimeAsset* Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, FReadyPlayerMeGlTFConfigCreator::GetGlTFRuntimeConfig());
	LoadSkeletalMesh(Asset, BodyType);
}

void UReadyPlayerMeGlbLoader::LoadSkeletalMesh(UglTFRuntimeAsset* Asset, EAvatarBodyType BodyType)
{
	if (Asset == nullptr)
	{
		(void)OnLoadCompleted.ExecuteIfBound(nullptr);
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to load the avatar model"));
	}
	if (BodyType == EAvatarBodyType::Undefined)
	{
		BodyType = FReadyPlayerMeMetadataExtractor::GetBodyTypeFromAsset(Asset);
	}
	const FString RootBoneName = FReadyPlayerMeMetadataExtractor::GetRootBoneName(BodyType);
	FReadyPlayerMeGlTFConfigCreator::OverrideConfig(SkeletalMeshConfig, RootBoneName, TargetSkeleton);
	Asset->LoadSkeletalMeshRecursiveAsync(RootBoneName, {}, OnSkeletalMeshCallback, SkeletalMeshConfig);
}

void UReadyPlayerMeGlbLoader::OnSkeletalMeshLoaded(USkeletalMesh* SkeletalMesh)
{
	(void)OnLoadCompleted.ExecuteIfBound(SkeletalMesh);
}
