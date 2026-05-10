// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObject/ListDataObject_Scalar.h"


FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;

}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringTofloat(DataDynamicGetter->GetValueAsString()));
		
	}

	return 0.f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue
		);

		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

		NotifyListDataModified(this);
	}
}

float UListDataObject_Scalar::StringTofloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;

	LexFromString(OutConvertedValue, *InString);

	return OutConvertedValue;
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringTofloat(GetDefaultValueAsString());
		const float CurrentValue = StringTofloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}
	
	return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}
	return false;
}

void UListDataObject_Scalar::OnEditDependencyDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	NotifyListDataModified(ModifiedData, ModifyReason);

	Super::OnEditDependencyDataModified(ModifiedData, ModifyReason);
}

