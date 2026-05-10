// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/MainCombatComponent.h"
#include "Item/Weapon/UndercoreMainWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UndercoreGameplayTags.h"

AUndercoreMainWeapon* UMainCombatComponent::GetMainCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AUndercoreMainWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AUndercoreMainWeapon* UMainCombatComponent::GetMainCurrentEquippedWeapon() const
{
    return Cast<AUndercoreMainWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UMainCombatComponent::GetMainCurrentEquippedDamageAtLevel(float InLevel) const
{
    return GetMainCurrentEquippedWeapon()->MainWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}



void UMainCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }

    OverlappedActors.AddUnique(HitActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        UndercoreGameplayTags::Shared_Event_MeleeHit,
        Data
    );


}

void UMainCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
