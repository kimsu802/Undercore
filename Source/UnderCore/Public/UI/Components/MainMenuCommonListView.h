// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "MainMenuCommonListView.generated.h"


class UDataAsset_DataListEntryMapping;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UMainMenuCommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;


private:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	UPROPERTY(EditAnywhere, Category = "MainMenu List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
};
