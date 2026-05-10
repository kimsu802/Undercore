// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Ability/UndercoreGameplayAbility.h"



void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel)
{
	check(InUndercoreASCToGive);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

			InUndercoreASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InUndercoreASCToGive->MakeEffectContext()
			);
		}
	}

	GrantAbilities(ActivateOnGivenAbilities, InUndercoreASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InUndercoreASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UUndercoreGameplayAbility>>& InAbilitiesToGive, UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
		return;

	for (const TSubclassOf<UUndercoreGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability)
			continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InUndercoreASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InUndercoreASCToGive->GiveAbility(AbilitySpec);
	}
}
