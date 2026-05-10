// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObject/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();
	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }

protected:
	virtual void OnDataObjectInitialized() override;

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;
};
