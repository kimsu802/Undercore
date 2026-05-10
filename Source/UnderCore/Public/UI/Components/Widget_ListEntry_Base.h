// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Type/UndercoreEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UCommonTextBlock;
class UListDataObject_Base;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void	BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected); 

	void	NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnToggleEntryWidgetHighlightState"))
	void	BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);

	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnToggleEditableState(bool bIsEditable);

	void SelectThisEntryWidget();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> CommonText_SettingDisplayName;

	UPROPERTY(Transient)
	UListDataObject_Base* CachedOwningDataObject;
};
