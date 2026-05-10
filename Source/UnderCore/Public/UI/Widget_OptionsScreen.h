// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget_ActivatableBase.h"
#include "Type/UndercoreEnumTypes.h"
#include "Widget_OptionsScreen.generated.h"


class UObtionsDataRegistry;
class UMainMenuTabListWidgetBase;
class UMainMenuCommonListView;
class UWidget_OptionsDetailView;
class UListDataObject_Base;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

private:
	UObtionsDataRegistry* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabId);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

	void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuTabListWidgetBase>	TabListWidget_OptionsTabs;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuCommonListView>		CommonListView_OptionsList;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget_OptionsDetailView>	DetailsView_ListEntryInfo;

	UPROPERTY(Transient)
	TObjectPtr<UObtionsDataRegistry> CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle	ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*>	ResettableDataArray;
	

	bool bIsResettingData = false;
};
