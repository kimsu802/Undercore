// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObject/ListDataObject_Collection.h"

void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	InChildListData->InitDataObject();

	InChildListData->SetParentData(this);

	ChildListDataArray.Add(InChildListData);
}
