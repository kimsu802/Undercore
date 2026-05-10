// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "ScalableFloat.h"
#include "DataAsset_MainStartUpData.generated.h"

class UInputMappingContext;
class UUnderCoreMainLinkedAnimLayer;
struct FUndercoreMainAbilitySet;
class UUndercoreMainLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FUndercoreMainAbilitySet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag	InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUndercoreGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};


USTRUCT(BlueprintType)
struct FUndercoreMainWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUndercoreMainLinkedAnimLayer>	WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext>			WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FUndercoreMainAbilitySet>			DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat								WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D>					SoftWeaponIconTexture;
};

/**
 * 
 */
UCLASS()
class UNDERCORE_API UDataAsset_MainStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UUndercoreAbilitySystemComponent* InUndercoreASCToGive, int32 ApplyLevel = 1) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FUndercoreMainAbilitySet> MainStartUpAbilitySets;
};
