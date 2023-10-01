// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeRenderLoader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Utils/ReadyPlayerMeRenderUrlConvertor.h"
#include "Request/ReadyPlayerMeBaseRequest.h"

//TODO: Move the timout to the RPMSettings to make it configurable
constexpr float IMAGE_REQUEST_TIMEOUT = 60.f;

void UReadyPlayerMeRenderLoader::Load(const FString& ModelUrl, const ERenderSceneType& SceneType, const TMap<EAvatarMorphTarget, float>& BlendShapes, const FDownloadImageCompleted& OnCompleted, const FDownloadImageFailed& OnFailed)
{
	OnDownloadImageCompleted = OnCompleted;
	OnDownloadImageFailed = OnFailed;
	const FString Url = FReadyPlayerMeRenderUrlConvertor::CreateRenderUrl(ModelUrl, SceneType, BlendShapes);

	ImageRequest = MakeShared<FReadyPlayerMeBaseRequest>();
	ImageRequest->GetCompleteCallback().BindUObject(this, &UReadyPlayerMeRenderLoader::OnImageDownloaded);
	ImageRequest->Download(Url, IMAGE_REQUEST_TIMEOUT);
}

void UReadyPlayerMeRenderLoader::OnImageDownloaded(bool bSuccess)
{
	if (!OnDownloadImageCompleted.IsBound())
	{
		return;
	}
	if (bSuccess)
	{
		UTexture2D* Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, ImageRequest->GetContent());
		(void)OnDownloadImageCompleted.ExecuteIfBound(Texture);
	}
	else
	{
		(void)OnDownloadImageFailed.ExecuteIfBound(TEXT("Failed to Download the image"));
	}
	ImageRequest.Reset();
}

void UReadyPlayerMeRenderLoader::BeginDestroy()
{
	if (ImageRequest.IsValid())
	{
		OnDownloadImageCompleted.Unbind();
		OnDownloadImageFailed.Unbind();
		ImageRequest->CancelRequest();
		ImageRequest.Reset();
	}

	Super::BeginDestroy();
}
