// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "ReadyPlayerMeTypes.h"
#include "ReadyPlayerMeRenderLoader.generated.h"

/**
 * Responsible for Loading rendered image from the avatar url.
 */
UCLASS(BlueprintType)
class READYPLAYERME_API UReadyPlayerMeRenderLoader : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Loads the rendered image.
	 * 
	 * @param ModelUrl Model url.
	 * @param SceneType The type of the scene where the avatar should be rendered.
	 * @param BlendShapes A map of the MorphTargets and values for them. BlendShapes can be used for applying facial expressions to the avatar.
	 * @param OnCompleted Success callback. Called when the render is loaded and provides the avatar texture as an argument.
	 * @param OnFailed Failure callback. If the render operation fails, the failure callback will be called.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Load", AutoCreateRefTerm = "BlendShapes, OnCompleted, OnFailed"))
	void Load(const FString& ModelUrl, const ERenderSceneType& SceneType, const TMap<EAvatarMorphTarget, float>& BlendShapes, const FDownloadImageCompleted& OnCompleted, const FDownloadImageFailed& OnFailed);

private:
	virtual void BeginDestroy() override;

	UFUNCTION()
	void OnImageDownloaded(bool bSuccess);

	TSharedPtr<class FReadyPlayerMeBaseRequest> ImageRequest;

	FDownloadImageCompleted OnDownloadImageCompleted;

	FDownloadImageFailed OnDownloadImageFailed;
};
