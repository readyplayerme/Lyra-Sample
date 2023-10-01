// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeFunctionLibrary.h"
#include "Storage/ReadyPlayerMeAvatarStorage.h"
#include "Utils/ReadyPlayerMeUrlConvertor.h"

void UReadyPlayerMeFunctionLibrary::ClearAvatarCache()
{
	FReadyPlayerMeAvatarStorage::ClearCache();
}

void UReadyPlayerMeFunctionLibrary::ClearAvatar(const FString& Guid)
{
    FReadyPlayerMeAvatarStorage::ClearAvatar(Guid);
}

bool UReadyPlayerMeFunctionLibrary::IsAvatarCacheEmpty()
{
    return FReadyPlayerMeAvatarStorage::IsCacheEmpty();
}

int32 UReadyPlayerMeFunctionLibrary::GetAvatarCount()
{
    return FReadyPlayerMeAvatarStorage::GetAvatarCount();
}

int64 UReadyPlayerMeFunctionLibrary::GetCacheSize()
{
    return FReadyPlayerMeAvatarStorage::GetCacheSize();
}

FString UReadyPlayerMeFunctionLibrary::GetAvatarGuid(const FString& UrlShortcode)
{
    return FReadyPlayerMeUrlConvertor::GetAvatarGuid(UrlShortcode);
}
