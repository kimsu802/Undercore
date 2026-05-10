// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ConfirmUI.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "UI/Components/MainMenuCommonButtonBase.h"
#include "ICommonInputModule.h"
#include "UndercoreFunctionLibrary.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->Title = InScreenTitle;
	InfoObject->Message = InScreenMsg;

	FConfirmButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_confirm"));

	InfoObject->AvailableButtons.Add(OKButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->Title = InScreenTitle;
	InfoObject->Message = InScreenMsg;

	FConfirmButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_yes"));

	FConfirmButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmButtonType::Closed;
	NoButtonInfo.ButtonTextToDisplay = UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_no"));


	InfoObject->AvailableButtons.Add(YesButtonInfo);
	InfoObject->AvailableButtons.Add(NoButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->Title = InScreenTitle;
	InfoObject->Message = InScreenMsg;

	FConfirmButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmButtonType::Confirmed;
	OKButtonInfo.ButtonTextToDisplay = UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_yes"));

	FConfirmButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cancel"));

	InfoObject->AvailableButtons.Add(OKButtonInfo);
	InfoObject->AvailableButtons.Add(CancelButtonInfo);

	return InfoObject;
}

void UWidget_ConfirmUI::InitConfirmUI(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmButtonType)> ClickedButtonCallback)
{
	check(InScreenInfoObject && CommonTextBlockTitle && CommonTextBlockMessage && DynamicEntryBoxButtons);

	CommonTextBlockTitle->SetText(InScreenInfoObject->Title);
	CommonTextBlockMessage->SetText(InScreenInfoObject->Message);

	if (DynamicEntryBoxButtons->GetNumEntries() != 0)
	{
		DynamicEntryBoxButtons->Reset<UMainMenuCommonButtonBase>(
			[](UMainMenuCommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}
		);
	}

	check(!InScreenInfoObject->AvailableButtons.IsEmpty());

	for (const FConfirmButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableButtons)
	{
		FDataTableRowHandle InputActionRowHandle;

		switch (AvailableButtonInfo.ConfirmScreenButtonType)
		{
			case EConfirmButtonType::Confirmed:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
				break;

			case EConfirmButtonType::Closed:
			case EConfirmButtonType::Cancelled:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
		}


		UMainMenuCommonButtonBase* AddedButton = DynamicEntryBoxButtons->CreateEntry<UMainMenuCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->SetTriggeringInputAction(InputActionRowHandle);
		AddedButton->OnClicked().AddLambda([ClickedButtonCallback, AvailableButtonInfo,this]() 
			{

			ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);

			DeactivateWidget();
		});
	}

	if (DynamicEntryBoxButtons->GetNumEntries() != 0)
	{
		DynamicEntryBoxButtons->GetAllEntries().Last()->SetFocus();
	}

}