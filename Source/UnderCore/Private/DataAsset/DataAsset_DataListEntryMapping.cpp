// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/Widget_ListEntry_Base.h"
#include "UI/DataObject/ListDataObject_Base.h"
#include "DataAsset/DataAsset_DataListEntryMapping.h"


TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClasByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);
	
	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			check(DataObjectListEntryMap.Contains(ConvertedDataObjectClass))

			return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
		}
	}

	return TSubclassOf<UWidget_ListEntry_Base>();
}
