// Copyright © 2021++ Ready Player Me


#include "ReadyPlayerMeFunctionLibrary.h"
#include "Storage/AvatarStorage.h"
#include "Utils/AvatarUrlConvertor.h"

void UReadyPlayerMeFunctionLibrary::ClearAvatarCache()
{
	FAvatarStorage::ClearCache();
}

void UReadyPlayerMeFunctionLibrary::ClearAvatar(const FString& Guid)
{
    FAvatarStorage::ClearAvatar(Guid);
}

bool UReadyPlayerMeFunctionLibrary::IsAvatarCacheEmpty()
{
    return FAvatarStorage::IsCacheEmpty();
}

int32 UReadyPlayerMeFunctionLibrary::GetAvatarCount()
{
    return FAvatarStorage::GetAvatarCount();
}

int64 UReadyPlayerMeFunctionLibrary::GetCacheSize()
{
    return FAvatarStorage::GetCacheSize();
}

FString UReadyPlayerMeFunctionLibrary::GetAvatarGuid(const FString& UrlShortcode)
{
    return FAvatarUrlConvertor::GetAvatarGuid(UrlShortcode);
}
