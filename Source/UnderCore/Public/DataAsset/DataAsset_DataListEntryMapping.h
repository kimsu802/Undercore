// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"


class UListDataObject_Base;
class UWidget_ListEntry_Base;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()


public:
	TSubclassOf<UWidget_ListEntry_Base> FindEntryWidgetClasByDataObject(UListDataObject_Base* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObject_Base>, TSubclassOf<UWidget_ListEntry_Base>> DataObjectListEntryMap;
	
};
