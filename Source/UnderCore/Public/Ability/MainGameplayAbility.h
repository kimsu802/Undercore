// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/UndercoreGameplayAbility.h"
#include "MainGameplayAbility.generated.h"


class AUndercoreMainCharacter;
class AUndercoreMainController;
class UMainCombatComponent;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UMainGameplayAbility : public UUndercoreGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	AUndercoreMainCharacter* GetMainCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	AUndercoreMainController* GetMainControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	UMainCombatComponent* GetMainCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Undercore|Ability")
	FGameplayEffectSpecHandle MakeMainDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

private:
	TWeakObjectPtr<AUndercoreMainCharacter> CachedUndercoreMainCharacter;
	TWeakObjectPtr<AUndercoreMainController> CachedUndercoreMainController;

};
