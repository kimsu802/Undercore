// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UndercoreAbilitySystemComponent.generated.h"


struct FUndercoreMainAbilitySet;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Undercore|Ability", meta = (ApplyLevel = 1))
	void GrantMainWeaponAbilities(const TArray<FUndercoreMainAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Undercore|Ability")
	void RemoveGrantedMainWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Undercore|Ability")
	bool TryActivateAbilityByTag(const FGameplayTag& AbilityTagToActivate);
};
