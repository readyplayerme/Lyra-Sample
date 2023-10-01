// Copyright © 2021++ Ready Player Me

#include "ReadyPlayerMeGameSubsystem.h"
#include "ReadyPlayerMeMemoryCache.h"

UReadyPlayerMeGameSubsystem::UReadyPlayerMeGameSubsystem()
	: MemoryCache(nullptr)
{}

void UReadyPlayerMeGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	MemoryCache = NewObject<UReadyPlayerMeMemoryCache>(this, TEXT("MemoryCache"));
}

void UReadyPlayerMeGameSubsystem::Deinitialize()
{
	MemoryCache = nullptr;
}
