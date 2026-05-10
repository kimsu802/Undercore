// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/UndercoreWeaponBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "DataAsset/DataAsset_MainStartUpData.h"
#include "UndercoreMainWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API AUndercoreMainWeapon : public AUndercoreWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FUndercoreMainWeaponData MainWeaponData;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles) { GrantedAbilitySpecHandles = InSpecHandles;  }

	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const { return GrantedAbilitySpecHandles;  }

private:
	TArray< FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
};
