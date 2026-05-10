// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Option/Widget_OptionsDetailView.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "UI/DataObject/ListDataObject_Base.h"

void UWidget_OptionsDetailView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
		return;

	if (CommonTextBlock_Title)
	{
		CommonTextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	}

	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (CommonRichTextBlock_Description)
	{
		CommonRichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());
	}

	if (CommonRichTextBlock_DynamicDetails)
	{
		const FString& DynamicDetails = FString::Printf(TEXT("Data Object Class : <Bold>%s</>\n\nEntry Widget Class : <Bold>%s</>"),
			*InDataObject->GetClass()->GetName(),
			*InEntryWidgetClassName);

		CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));
	}

	if (CommonRichTextBlock_DisabledReason)
	{
		CommonRichTextBlock_DisabledReason->SetText(InDataObject->IsDataCurrentlyEditable() ? FText::GetEmpty() : InDataObject->GetDisabledRichText());
	}
}

void UWidget_OptionsDetailView::ClearDetailsViewInfo()
{
	if (CommonTextBlock_Title)
	{
		CommonTextBlock_Title->SetText(FText::GetEmpty());
	}
	if (CommonRichTextBlock_Description)
	{
		CommonRichTextBlock_Description->SetText(FText::GetEmpty());
	}
	if (CommonRichTextBlock_DynamicDetails)
	{
		CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	}
	if (CommonRichTextBlock_DisabledReason)
	{
		CommonRichTextBlock_DisabledReason->SetText(FText::GetEmpty());
	}
	if (CommonLazyImage_DescriptionImage)
	{
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidget_OptionsDetailView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();
}
