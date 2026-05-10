// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Type/UndercoreEnumTypes.h"
#include "UndercoreQuestStepDataTable.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreQuestStepDataTable : public UDataTable
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FUndercoreQuestStepRow : public FTableRowBase
{
    GENERATED_BODY()

    // 어떤 퀘스트에 속하는지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestID;

    // 몇 번째 단계인지 (정렬용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StepIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestConditionType ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetTag;

    UPROPERTY(EditAnywhere)
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetEnemiesNum = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AcceptRadius = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSpawnEnemy = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> TargetEnemyClass;
};
