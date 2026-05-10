// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "MainMenuDeveloperSettings.generated.h"


class UWidget_ActivatableBase;
class UUndercoreBanWordDataTable;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "UI Settings"))
class UNDERCORE_API UMainMenuDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "FrontEnd.Widget,Push.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow))
	TSoftClassPtr<UWidget_ActivatableBase>	DialogueWidget;

	UPROPERTY(Config, EditAnywhere, Category = "Banned Word Datatable Reference", meta = (ForceInlineRow))
	TSoftObjectPtr<UDataTable>	BannedWordDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Dialogue Datatable Reference", meta = (ForceInlineRow))
	TSoftObjectPtr<UDataTable>	DialogueDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Quest Datatable Reference", meta = (ForceInlineRow))
	TSoftObjectPtr<UDataTable>	QuestDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Item Datatable Reference", meta = (ForceInlineRow))
	TSoftObjectPtr<UDataTable>	ItemDataTable;

	UPROPERTY(Config, EditAnywhere, Category = "Push Widget Maximum")
	int32	MaximumPushWidgetNum = 5;
};
