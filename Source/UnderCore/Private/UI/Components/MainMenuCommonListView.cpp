// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/MainMenuCommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/Components/Widget_ListEntry_Base.h"
#include "DataAsset/DataAsset_DataListEntryMapping.h"
#include "UI/DataObject/ListDataObject_Base.h"
#include "UI/DataObject/ListDataObject_Collection.h"



UUserWidget& UMainMenuCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	
	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClasByDataObject(CastChecked<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}
bool UMainMenuCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UListDataObject_Collection>();
}

#if WITH_EDITOR
void UMainMenuCommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable DataListEntryMapping has no valid data asset assigned.") +
			GetClass()->GetName() +
			TEXT(" needs a valid data asset to function properly")
		));
	}
}
#endif
