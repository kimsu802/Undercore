// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/UndercoreGameplayAbility.h"
#include "UndercoreEnemyGameplayAbility.generated.h"


class AUndercoreEnemyCharacter;
class UEnemyCombatComponent;


/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreEnemyGameplayAbility : public UUndercoreGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	AUndercoreEnemyCharacter*	GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	UEnemyCombatComponent*		GetEnemyCombatComponentFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	FGameplayEffectSpecHandle	MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<AUndercoreEnemyCharacter>	CachedUndercoreEnemyCharacter;
};
