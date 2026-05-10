// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/UndercoreEnemyGameplayAbility.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Character/UndercoreEnemyCharacter.h"
#include "UndercoreGameplayTags.h"

AUndercoreEnemyCharacter* UUndercoreEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (!CachedUndercoreEnemyCharacter.IsValid())
    {
        CachedUndercoreEnemyCharacter = CastChecked<AUndercoreEnemyCharacter>(GetAvatarActorFromActorInfo());
    }

    return CachedUndercoreEnemyCharacter.Get();
}

UEnemyCombatComponent* UUndercoreEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UUndercoreEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    FGameplayEffectContextHandle ContextHandle = GetUndercoreAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetUndercoreAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle);

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        UndercoreGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );


    return EffectSpecHandle;
}
