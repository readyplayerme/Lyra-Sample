// Copyright © 2023++ Ready Player Me


#include "RpmAvatarTemplateDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/AvatarTemplateExtractor.h"

namespace
{
	bool IsAvatarFiltered(EAvatarGender Gender, EAvatarGender SelectedGender)
	{
		return Gender == SelectedGender || SelectedGender == EAvatarGender::Undefined;
	}
}

void URpmAvatarTemplateDownloader::SetRequestFactory(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

void URpmAvatarTemplateDownloader::DownloadTemplates(EAvatarGender Gender, const FAvatarTemplatesDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	OnDownloadCompleted = DownloadCompleted;
	OnFailed = Failed;
	SelectedGender = Gender;
	if (AvatarTemplates.Num() != 0)
	{
		DownloadImages();
	}
	else
	{
		AvatarTemplatesRequest = RequestFactory->CreateAvatarTemplatesRequest();
		AvatarTemplatesRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarTemplateDownloader::OnTemplatesDownloadCompleted);
		AvatarTemplatesRequest->Download();
	}
}

bool URpmAvatarTemplateDownloader::IsValidAvatarTemplate(const FString& ImageUrl, EAvatarGender Gender) const
{
	return ImageMap.Contains(ImageUrl) && IsAvatarFiltered(Gender, SelectedGender);
}

TArray<FRpmAvatarTemplate> URpmAvatarTemplateDownloader::GetFilteredAvatarTemplates() const
{
	return AvatarTemplates.FilterByPredicate([this](const auto& Template){ return IsValidAvatarTemplate(Template.ImageUrl, Template.Gender); });
}

bool URpmAvatarTemplateDownloader::AreAvatarsReady() const
{
	return ImageRequests.Num() == 0 && AvatarTemplates.Num() > 0;
}

void URpmAvatarTemplateDownloader::OnTemplatesDownloadCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::AvatarTemplateFailure);
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
		return;
	}
	AvatarTemplates = FAvatarTemplateExtractor::ExtractAvatarTemplates(AvatarTemplatesRequest->GetContentAsString());
	AvatarTemplatesRequest.Reset();
	DownloadImages();
}

void URpmAvatarTemplateDownloader::DownloadImages()
{
	for (const auto& Template : AvatarTemplates)
	{
		if (ImageMap.Contains(Template.ImageUrl) || !IsAvatarFiltered(Template.Gender, SelectedGender))
		{
			continue;
		}

		auto IconRequest = RequestFactory->CreateImageRequest(Template.ImageUrl);
		ImageRequests.Add(Template.ImageUrl, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarTemplateDownloader::OnImageDownloadCompleted, Template.ImageUrl);
		IconRequest->Download();
	}
	if (ImageRequests.Num() == 0)
	{
		(void)OnDownloadCompleted.ExecuteIfBound(GetFilteredAvatarTemplates());
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
	}
}

void URpmAvatarTemplateDownloader::OnImageDownloadCompleted(bool bSuccess, FString ImageUrl)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::AvatarTemplateFailure);
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
		// Check if we need to stop downloading other assets
		return;
	}

	UTexture2D* Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, ImageRequests[ImageUrl]->GetContent());
	ImageRequests.Remove(ImageUrl);
	ImageMap.Add(ImageUrl, Texture);
	FRpmAvatarTemplate* Template = AvatarTemplates.FindByPredicate([ImageUrl](const auto& Template){return ImageUrl == Template.ImageUrl;});
	Template->Image = Texture;

	if (ImageRequests.Num() == 0)
	{
		(void)OnDownloadCompleted.ExecuteIfBound(GetFilteredAvatarTemplates());
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
	}
}
