// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailView.generated.h"

class UCommonTextBlock;
class UCommonRichTextBlock;
class UCommonLazyImage;
class UListDataObject_Base;

/**
 * 
 */
UCLASS(BlueprintType, Abstract, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_OptionsDetailView : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonRichTextBlock> CommonRichTextBlock_Description;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonRichTextBlock> CommonRichTextBlock_DynamicDetails;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonRichTextBlock> CommonRichTextBlock_DisabledReason;
};
