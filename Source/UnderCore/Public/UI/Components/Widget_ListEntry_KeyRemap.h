// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UMainMenuCommonButtonBase;
class UListDataObject_KeyRemap;

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick), BlueprintType)
class UNDERCORE_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	
private:
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = true))
	TObjectPtr<UMainMenuCommonButtonBase> CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = true))
	TObjectPtr<UMainMenuCommonButtonBase> CommonButton_ResetKeyBinding;
	
	UPROPERTY(Transient)
	UListDataObject_KeyRemap* CachedOwningKeyRemapDataObject;
};
