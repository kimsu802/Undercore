// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/Widget_ListEntry_KeyRemap.h"
#include "UI/DataObject/ListDataObject_KeyRemap.h"
#include "UI/Components/MainMenuCommonButtonBase.h"
#include "Manager/MainMenuUISubsystem.h"
#include "UndercoreGameplayTags.h"
#include "UI/Widget_KeyRemapScreen.h"
#include "UndercoreFunctionLibrary.h"



void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonButton_RemapKey)
	{
		CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	}

	if (CommonButton_ResetKeyBinding)
	{
		CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
	}
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();

	UMainMenuUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		UndercoreGameplayTags::FrontEnd_WidgetStack_Modal,
		UUndercoreFunctionLibrary::GetFrontendSoftWidgetClassByTag(UndercoreGameplayTags::FrontEnd_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);

				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetCachedDesiredInputKeyType());
				}
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();
	
	check(CachedOwningKeyRemapDataObject);

	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UMainMenuUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmUIType::OK,
			FText::GetEmpty(),
			FText::FromString(TEXT("이미 기본값 입니다.")),
			[](EConfirmButtonType ClickedButton)
			{

			}
		);

		return;
	}


	UMainMenuUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmUIType::YesNo,
		FText::GetEmpty(),
		FText::FromString(TEXT("기본 키 값으로 바꾸시겠습니까?")),
		[this](EConfirmButtonType ClickedButton)
		{
			if (ClickedButton == EConfirmButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UMainMenuUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmUIType::OK,
		FText::GetEmpty(),
		FText::FromString(CanceledReason),
		[](EConfirmButtonType ClickedButton) { }
	);
}
