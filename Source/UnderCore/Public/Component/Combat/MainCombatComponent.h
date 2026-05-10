// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Combat/PawnCombatComponent.h"
#include "MainCombatComponent.generated.h"

class AUndercoreMainWeapon;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UMainCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Undercore|Combat")
	AUndercoreMainWeapon* GetMainCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Undercore|Combat")
	AUndercoreMainWeapon* GetMainCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Undercore|Combat")
	float GetMainCurrentEquippedDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
