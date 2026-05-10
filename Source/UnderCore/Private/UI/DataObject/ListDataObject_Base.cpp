// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObject/ListDataObject_Base.h"
#include "Settings/UndercoreGameUserSettings.h"
#include "Type/UndercoreStructTypes.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(this, &ThisClass::OnEditDependencyDataModified);
	}
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		Condition.GetDisabledRichReason();

		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();

			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;

}


void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediately)
	{
		UUndercoreGameUserSettings::Get()->ApplySettings(true);
	}
}

void UListDataObject_Base::OnEditDependencyDataModified(UListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnDependencyDataModified.Broadcast(ModifiedData, ModifyReason);
}
