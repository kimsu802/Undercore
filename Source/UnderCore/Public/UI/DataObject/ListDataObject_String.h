// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObject/ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	void AdvanceToNextOption();
	void BackToPreviousOption();

protected:
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) override;

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);

	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	FORCEINLINE const TArray<FText> GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText;  }

};

UCLASS()
class UNDERCORE_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:
	virtual void OnDataObjectInitialized() override;

private:
	void TryInitBoolValues();

	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

UCLASS()
class UNDERCORE_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOptions(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}
};

UCLASS()
class UNDERCORE_API UListDataObject_StringInteger : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDisplayText);

protected:
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependencyDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason) override;

};