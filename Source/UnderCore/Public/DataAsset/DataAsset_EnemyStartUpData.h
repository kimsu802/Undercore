// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"


class UUndercoreEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InASCToGive, int ApplyLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray <TSubclassOf<UUndercoreEnemyGameplayAbility>> EnemyCombatAbilities;
};
