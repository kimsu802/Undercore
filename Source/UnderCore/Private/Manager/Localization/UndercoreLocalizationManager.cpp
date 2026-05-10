// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Localization/UndercoreLocalizationManager.h"

UUndercoreLocalizationManager* UUndercoreLocalizationManager::Get(const UObject* WorldContextObject)
{
	return UGameInstance::GetSubsystem<UUndercoreLocalizationManager>(WorldContextObject->GetWorld()->GetGameInstance());
}