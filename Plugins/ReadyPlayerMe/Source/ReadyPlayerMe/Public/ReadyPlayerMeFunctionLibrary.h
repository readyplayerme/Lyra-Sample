// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadyPlayerMeFunctionLibrary.generated.h"

/**
 * Static functions to be used from the blueprint.
 */
UCLASS()
class READYPLAYERME_API UReadyPlayerMeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Clears all avatars from the persistent cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Clear Avatar Cache"))
	static void ClearAvatarCache();

	/** Clears a specific avatar from persistent cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Clear Avatar"))
	static void ClearAvatar(const FString& Guid);

	/** Is there any avatars present in the persistent cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Is Avatar Cache Empty"))
	static bool IsAvatarCacheEmpty();

	/** Total Avatars stored in persistent cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Avatar Count"))
	static int32 GetAvatarCount();

	/** Total Avatars stored in persistent cache. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Cache Size"))
	static int64 GetCacheSize();

	/** Get unique id of the avatar. */
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Avatar Guid"))
	static FString GetAvatarGuid(const FString& UrlShortcode);
};
