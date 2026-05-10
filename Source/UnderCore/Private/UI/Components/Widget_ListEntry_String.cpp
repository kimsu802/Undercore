// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/Widget_ListEntry_String.h"
#include "UI/DataObject/ListDataObject_String.h"
#include "UI/Components/MainMenuCommonRotator.h"
#include "UI/Components/MainMenuCommonButtonBase.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	CommonButton_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);
}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionsByText(CachedOwningStringDataObject->GetCurrentDisplayText());

}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionsByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	CommonButton_Previous->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_Next->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_String::OnPreviousButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}
	
	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnNextButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisEntryWidget();
}
