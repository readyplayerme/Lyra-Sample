// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmAvatarTemplateDownloader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmAvatarTemplateDownloader : public UObject
{
	GENERATED_BODY()

public:
	void DownloadTemplates(EAvatarGender Gender, const FAvatarTemplatesDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed);

	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	TArray<FRpmAvatarTemplate> GetFilteredAvatarTemplates() const;

	bool AreAvatarsReady() const;

private:
	void OnTemplatesDownloadCompleted(bool bSuccess);

	void OnImageDownloadCompleted(bool bSuccess, FString AvatarId);

	void DownloadImages();
	
	bool IsValidAvatarTemplate(const FString& Id, EAvatarGender Gender) const;

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	FAvatarTemplatesDownloadCompleted OnDownloadCompleted;
	FAvatarCreatorFailed OnFailed;

	EAvatarGender SelectedGender;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TArray<FRpmAvatarTemplate> AvatarTemplates;
	TSharedPtr<class IBaseRequest> AvatarTemplatesRequest;
	TMap<FString, TSharedPtr<class IBaseRequest>> ImageRequests;
};
