// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_EnemyStartUpData.h"
#include "Ability/UndercoreEnemyGameplayAbility.h"
#include "Ability/UndercoreAbilitySystemComponent.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InASCToGive, int ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UUndercoreEnemyGameplayAbility>& Ability : EnemyCombatAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
