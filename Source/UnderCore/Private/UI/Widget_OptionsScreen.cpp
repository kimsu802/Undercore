// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_OptionsScreen.h"
#include "UI/Components/MainMenuTabListWidgetBase.h"
#include "UI/Components/MainMenuCommonListView.h"
#include "UI/DataObject/ObtionsDataRegistry.h"
#include "UI/DataObject/ListDataObject_Collection.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "UndercoreDebugHelper.h"
#include "Settings/UndercoreGameUserSettings.h"
#include "UI/Components/Widget_ListEntry_Base.h"
#include "UI/Option/Widget_OptionsDetailView.h"
#include "Manager/MainMenuUISubsystem.h"
#include "UI/Components/MainMenuCommonButtonBase.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UndercoreFunctionLibrary.h"




void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	if (!ResetAction.IsNull())
	{
		ResetActionHandle  = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			));
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		));

	if (TabListWidget_OptionsTabs)
	{
		TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
	}

	if (CommonListView_OptionsList)
	{
		CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
		CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
	}
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
			continue;

		if (TabListWidget_OptionsTabs)
		{
			const FName TabID = TabCollection->GetDataID();
			if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))
			{
				continue;
			}

			TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
		}
	}
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();	
	
	UUndercoreGameUserSettings::Get()->ApplySettings(true);
}

UObtionsDataRegistry* UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UObtionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());

	const FString SelectedTabButtonName = CastChecked<UMainMenuCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UMainMenuUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmUIType::YesNo,
		FText::GetEmpty(),
		FText::Format(FTextFormat(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("confirm_reset_options"))),FText::FromString(SelectedTabButtonName)),
		[this](EConfirmButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;

			for (UListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				if (DataToReset->TryResetBackToDefaultValue())
				{

				}
				else
				{
					bHasDataFailedToReset = true;
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}

			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	if (UUndercoreGameUserSettings::Get()->GetCurrentLanguageSetting() != UKismetInternationalizationLibrary::GetCurrentLanguage())
	{
		UMainMenuUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmUIType::YesNo,
			FText::GetEmpty(),
			UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("confirm_restart_language")),
			[this](EConfirmButtonType type)
			{
				if (type == EConfirmButtonType::Confirmed)
				{
					UKismetInternationalizationLibrary::SetCurrentLanguage(UUndercoreGameUserSettings::Get()->GetCurrentLanguageSetting());

					UUndercoreGameUserSettings::Get()->ApplySettings(true);

					UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
				}

				if (type == EConfirmButtonType::Closed)
				{
					UUndercoreGameUserSettings::Get()->SetCurrentLanguageSetting(UUndercoreGameUserSettings::Get()->GetCurrentLanguageSetting());
				}
			}
		);

		return;
	}

	DeactivateWidget();
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabId)
{
	DetailsView_ListEntryInfo->ClearDetailsViewInfo();

	TArray<UListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabId);

	check(CommonListView_OptionsList);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
		return;

	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		if (DetailsView_ListEntryInfo)
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
				CastChecked<UListDataObject_Base>(InHoveredItem),
				TryGetEntryWidgetClassName(InHoveredItem)
				);
		}
	}

	else
	{
		if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
				SelectedItem,
				TryGetEntryWidgetClassName(SelectedItem)
			);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
		return;

	if (DetailsView_ListEntryInfo)
	{
		DetailsView_ListEntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InSelectedItem),
			TryGetEntryWidgetClassName(InSelectedItem)
		);
	}
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
		
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
