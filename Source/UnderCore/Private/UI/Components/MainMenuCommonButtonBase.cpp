// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/MainMenuCommonButtonBase.h"
#include "Manager/MainMenuUISubsystem.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"

void UMainMenuCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

FText UMainMenuCommonButtonBase::GetButtonDisplayText() const
{
	if (CommonTextBlock_ButtonText)
	{
		return CommonTextBlock_ButtonText->GetText();
	}

	return FText();
}

void UMainMenuCommonButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetBrush(InBrush);
	}
}

void UMainMenuCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UMainMenuCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UMainMenuCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		if (UMainMenuUISubsystem* System = UMainMenuUISubsystem::Get(this))
		{
			System->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
		}
	}
}

void UMainMenuCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (UMainMenuUISubsystem* System = UMainMenuUISubsystem::Get(this))
	{
		System->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
	}
}
