// Copyright © 2021++ Ready Player Me


#include "AvatarStorage.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/FileManagerGeneric.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION > 4
#include "HAL/PlatformFileManager.h"
#else
#include "HAL/PlatformFilemanager.h"
#endif

namespace
{
	FString GetAvatarCacheDir()
	{
		return FPaths::ProjectPersistentDownloadDir() + "/Avatars/";
	}
}

FString FAvatarStorage::LoadMetadata(const FString& Path)
{
	if (!Path.IsEmpty() && FPaths::FileExists(*Path))
	{
		FString ResultStr;
		if (FFileHelper::LoadFileToString(ResultStr, *Path))
		{
			return ResultStr;
		}
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to load the metadata"));
	}
	return "";
}

bool FAvatarStorage::AvatarExists(const FAvatarUri& AvatarUri)
{
	return FileExists(AvatarUri.LocalMetadataPath) && FileExists(AvatarUri.LocalModelPath);
}

bool FAvatarStorage::FileExists(const FString& Path)
{
	return !Path.IsEmpty() && FPaths::FileExists(*Path);
}

void FAvatarStorage::DeleteDirectory(const FString& Path)
{
	if (Path.IsEmpty())
	{
		return;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.DirectoryExists(*Path) && !PlatformFile.DeleteDirectoryRecursively(*Path))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to delete directory"));
	}
}

bool FAvatarStorage::CheckAndRemoveExistingFile(const FString& FilePath)
{
	if (FilePath.IsEmpty())
	{
		return false;
	}
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString Path;
	FString Filename;
	FString Extension;
	FPaths::Split(FilePath, Path, Filename, Extension);
	if (!PlatformFile.DirectoryExists(*Path) && !PlatformFile.CreateDirectoryTree(*Path))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to create a directory to save the downloaded file"));
		return false;
	}

	if (PlatformFile.FileExists(*FilePath) && !PlatformFile.DeleteFile(*FilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to delete the existing file"));
		return false;
	}
	return true;
}

void FAvatarStorage::SaveAvatar(const FString& GlbFilePath, const TArray<uint8>& Data)
{
	if (!CheckAndRemoveExistingFile(GlbFilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to delete the cached avatar model"));
	}
	if (!FFileHelper::SaveArrayToFile(Data, *GlbFilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the downloaded file"));
	}
}

void FAvatarStorage::SaveMetadata(const FString& MetadataFilePath, const FString& Content)
{
	if (!CheckAndRemoveExistingFile(MetadataFilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to delete the cached avatar metadata"));
	}
	if (!FFileHelper::SaveStringToFile(Content, *MetadataFilePath))
	{
		UE_LOG(LogReadyPlayerMe, Warning, TEXT("Failed to save the downloaded file"));
	}
}

void FAvatarStorage::ClearCache()
{
	DeleteDirectory(GetAvatarCacheDir());
}

bool FAvatarStorage::IsCacheEmpty()
{
	return GetAvatarCount() == 0;
}

void FAvatarStorage::ClearAvatar(const FString& Guid)
{
	DeleteDirectory(GetAvatarCacheDir() + Guid);
}

int32 FAvatarStorage::GetAvatarCount()
{
	const FString Path = GetAvatarCacheDir() + "*";
	TArray<FString> FoundDirs;
	IFileManager::Get().FindFiles(FoundDirs, *Path, false, true);
	return FoundDirs.Num();
}

int64 FAvatarStorage::GetCacheSize()
{
	int64 DirectorySize = 0;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.IterateDirectoryRecursively(*GetAvatarCacheDir(),
		[&DirectorySize, &PlatformFile](const TCHAR* Filename, bool bIsDirectory) -> bool
		{
			if (!bIsDirectory)
			{
				DirectorySize += PlatformFile.FileSize(Filename);
			}
			return true;
		});
	return DirectorySize;
}
