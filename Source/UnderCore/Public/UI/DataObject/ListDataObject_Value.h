// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObject/ListDataObject_Base.h"
#include "UI/Option/OptionsDataInteractionHelper.h"
#include "ListDataObject_Value.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNDERCORE_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue;  }
	
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }
protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }

	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;
	
private:
	TOptional<FString> DefaultStringValue;
};
