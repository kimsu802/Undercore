// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UndercoreSaveGame.generated.h"

struct FQuestStep;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString		PlayerNickName;

	UPROPERTY(BlueprintReadWrite)
	FVector		PlayerLocation;

	UPROPERTY(BlueprintReadWrite)
	FString		CurrentDate;

	UPROPERTY(BlueprintReadWrite)
	FString		CurrentTime;

	UPROPERTY(BlueprintReadWrite)
	TArray<FQuestStep>	CurrentQuestSteps;

	UPROPERTY(BlueprintReadWrite)
	int32				CurrentQuestStepIndex;
};
