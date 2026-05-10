// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Manager/Quest/UndercoreQuestManager.h"
#include "UndercorePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API AUndercorePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetNickName(FString InName) { NickName = InName; }
	FORCEINLINE const FString& GetNickName() { return NickName; }

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FString NickName;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FQuestStep	CurrentQuestStep;
};
