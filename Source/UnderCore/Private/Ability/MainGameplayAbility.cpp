// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/MainGameplayAbility.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Component/Combat/MainCombatComponent.h"
#include "Character/UndercoreMainCharacter.h"
#include "Controller/UndercoreMainController.h"
#include "UndercoreGameplayTags.h"

AUndercoreMainCharacter* UMainGameplayAbility::GetMainCharacterFromActorInfo()
{
	if (!CachedUndercoreMainCharacter.IsValid())
	{
		CachedUndercoreMainCharacter = Cast<AUndercoreMainCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedUndercoreMainCharacter.IsValid() ? CachedUndercoreMainCharacter.Get() : nullptr;
}

AUndercoreMainController* UMainGameplayAbility::GetMainControllerFromActorInfo()
{
	if (!CachedUndercoreMainController.IsValid())
	{
		CachedUndercoreMainController = Cast<AUndercoreMainController>(CurrentActorInfo->PlayerController);
	}

	return CachedUndercoreMainController.IsValid() ? CachedUndercoreMainController.Get() : nullptr;
}

UMainCombatComponent* UMainGameplayAbility::GetMainCombatComponentFromActorInfo()
{
	return GetMainCharacterFromActorInfo()->GetMainCombatComponent();
}


FGameplayEffectSpecHandle UMainGameplayAbility::MakeMainDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{ 
	check(EffectClass);
	FGameplayEffectContextHandle ContextHandle = GetUndercoreAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetUndercoreAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		UndercoreGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}

	return EffectSpecHandle;
}