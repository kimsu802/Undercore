// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UndercoreItemDataManager.generated.h"

struct FUndercoreItemData;
/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreItemDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UUndercoreItemDataManager* Get(UObject* WorldContextObject);

	const FUndercoreItemData* FindItemDataByID(FName ItemID);

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY()
	TObjectPtr<UDataTable>	CachedItemDataTable;
};
