// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UndercoreItemDataManager.h"
#include "Settings/MainMenuDeveloperSettings.h"
#include "DataTable/UndercoreItemDataTable.h"

UUndercoreItemDataManager* UUndercoreItemDataManager::Get(UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UUndercoreItemDataManager>(World->GetGameInstance());
	}
	return nullptr;
}

const FUndercoreItemData* UUndercoreItemDataManager::FindItemDataByID(FName ItemID)
{
	if (CachedItemDataTable)
	{
		return CachedItemDataTable->FindRow<FUndercoreItemData>(ItemID,"");
	}

	return nullptr;
}

void UUndercoreItemDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UMainMenuDeveloperSettings* MainMenuDeveloperSettings = GetDefault<UMainMenuDeveloperSettings>())
	{
		CachedItemDataTable = MainMenuDeveloperSettings->ItemDataTable.LoadSynchronous();
	}
}
