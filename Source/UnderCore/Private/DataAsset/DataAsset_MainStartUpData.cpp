// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_MainStartUpData.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Ability/UndercoreGameplayAbility.h"


void UDataAsset_MainStartUpData::GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InUndercoreASCToGive, ApplyLevel);

	for (const FUndercoreMainAbilitySet& AbilitySet : MainStartUpAbilitySets)
	{
		if (!AbilitySet.IsValid())
			continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InUndercoreASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		InUndercoreASCToGive->GiveAbility(AbilitySpec);
	}
}

bool FUndercoreMainAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;	
}
