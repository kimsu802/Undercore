// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
class UListDataObject_Scalar;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnSliderMouseCaptureBegin();


private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonNumericTextBlock> CommonNumericTextBlock_SettingValue;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UAnalogSlider>			AnalogSlider_SettingSlider;

	UPROPERTY(Transient)
	UListDataObject_Scalar*				CachedOwningScalarDataObject;
};
