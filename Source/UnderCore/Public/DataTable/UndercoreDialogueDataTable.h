// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Type/UndercoreStructTypes.h"
#include "UndercoreDialogueDataTable.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreDialogueDataTable : public UDataTable
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FUndercoreDialogueRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueLine>	DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOnFinishedBlend = false;

};
