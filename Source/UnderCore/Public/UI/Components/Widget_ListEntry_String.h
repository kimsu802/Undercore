// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UMainMenuCommonButtonBase;
class UMainMenuCommonRotator;
class UListDataObject_String;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnToggleEditableState(bool bIsEditable) override;


private:
	void OnPreviousButtonClicked();
	void OnNextButtonClicked();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UMainMenuCommonButtonBase> CommonButton_Next;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UMainMenuCommonButtonBase> CommonButton_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UMainMenuCommonRotator> CommonRotator_AvailableOptions;

	UPROPERTY(Transient)
	TObjectPtr<UListDataObject_String> CachedOwningStringDataObject;
};
