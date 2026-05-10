// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UndercoreItemDataTable.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreItemDataTable : public UDataTable
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FUndercoreItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table ID")
	FString						Description_key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table ID")
	TSoftObjectPtr<UTexture2D>	ItemImage;
};