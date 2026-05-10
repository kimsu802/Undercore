// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Option/Widget_ListEntry_Scalar.h"
#include "UI/DataObject/ListDataObject_Scalar.h"
#include "CommonNumericTextBlock.h"
#include "CommonTextBlock.h"
#include "AnalogSlider.h"

void UWidget_ListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (AnalogSlider_SettingSlider)
	{
		AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
		AnalogSlider_SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this, &ThisClass::OnSliderMouseCaptureBegin);
	}
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningScalarDataObject = CastChecked<UListDataObject_Scalar>(InOwningListDataObject);

	if (CommonNumericTextBlock_SettingValue)
	{
		CommonNumericTextBlock_SettingValue->SetNumericType(CachedOwningScalarDataObject->GetDisplayNumericType());
		CommonNumericTextBlock_SettingValue->FormattingSpecification = CachedOwningScalarDataObject->GetNumberFormattingOptions();
		CommonNumericTextBlock_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
	}

	if (AnalogSlider_SettingSlider)
	{
		AnalogSlider_SettingSlider->SetMinValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
		AnalogSlider_SettingSlider->SetMaxValue(CachedOwningScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
		AnalogSlider_SettingSlider->SetStepSize(CachedOwningScalarDataObject->GetSliderStepSize());
		AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
	}
}

void UWidget_ListEntry_Scalar::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningScalarDataObject)
	{
		CommonNumericTextBlock_SettingValue->SetCurrentValue(CachedOwningScalarDataObject->GetCurrentValue());
	}

	if (AnalogSlider_SettingSlider)
	{
		AnalogSlider_SettingSlider->SetValue(CachedOwningScalarDataObject->GetCurrentValue());
	}
}

void UWidget_ListEntry_Scalar::OnSliderValueChanged(float Value)
{
	if (CachedOwningScalarDataObject)
	{
		CachedOwningScalarDataObject->SetCurrentValueFromSlider(Value);
	}
}

void UWidget_ListEntry_Scalar::OnSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}

