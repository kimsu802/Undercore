// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystemBlueprintLibrary.h"
#include "Component/Combat/EnemyCombatComponent.h"
#include "Components/BoxComponent.h"
#include "UndercoreGameplayTags.h"
#include "Character/UndercoreEnemyCharacter.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
 {
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{

	}
	
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;


	if (bIsValidBlock)
	{

	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			UndercoreGameplayTags::Shared_Event_MeleeHit,
			Data
		);
	}

}

void UEnemyCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractredActor)
{
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AUndercoreEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AUndercoreEnemyCharacter>();

	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
		case EToggleDamageType::LeftHand:
			LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
			break;
		case EToggleDamageType::RightHand:
			RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
			break;
		default:
			break;
	}
	
	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}

