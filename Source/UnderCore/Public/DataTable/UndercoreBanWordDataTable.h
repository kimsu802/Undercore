// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UndercoreBanWordDataTable.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreBanWordDataTable : public UDataTable
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FUndercoreBanWordData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BannedWord");
    FString BanWord;
};
