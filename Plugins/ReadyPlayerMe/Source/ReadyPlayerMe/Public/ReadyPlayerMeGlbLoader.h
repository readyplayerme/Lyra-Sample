// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"
#include "glTFRuntimeParser.h"
#include "ReadyPlayerMeGlbLoader.generated.h"

UCLASS(BlueprintType)
class READYPLAYERME_API UReadyPlayerMeGlbLoader : public UObject
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	UReadyPlayerMeGlbLoader();

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Load Glb From File"))
	void LoadFromFile(const FString& LocalModelPath, EAvatarBodyType BodyType, const FGlbLoadCompleted& LoadCompleted);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Load Glb From Data"))
	void LoadFromData(const TArray<uint8>& Data, EAvatarBodyType BodyType, const FGlbLoadCompleted& LoadCompleted);

	/** It provides a flexibility to chose the skeleton that will be used for the loaded avatar.
	 * If it's not set the default skeleton will be used for the loaded avatar. */
	UPROPERTY(BlueprintReadWrite, Category="Ready Player Me")
	USkeleton* TargetSkeleton;

	/**
	 * glTFRuntime skeletal mesh config that will be used for loading the avatar.
	 * This property should be changed only for very custom cases.
	 */
	UPROPERTY(BlueprintReadWrite, Category="Ready Player Me")
	FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig;

private:
	UFUNCTION()
	void OnSkeletalMeshLoaded(USkeletalMesh* SkeletalMesh);

	void LoadSkeletalMesh(class UglTFRuntimeAsset* Asset, EAvatarBodyType BodyType);

	FglTFRuntimeSkeletalMeshAsync OnSkeletalMeshCallback;
	FGlbLoadCompleted OnLoadCompleted;
};
