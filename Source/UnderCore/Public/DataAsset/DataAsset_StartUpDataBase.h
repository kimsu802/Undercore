// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UUndercoreGameplayAbility;
class UUndercoreAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UUndercoreGameplayAbility>>	ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UUndercoreGameplayAbility>>	ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UUndercoreGameplayAbility>>& InAbilitiesToGive, UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel = 1);
};
