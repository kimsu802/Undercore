// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "UndercoreAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UUndercoreAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UUndercoreAttributeSet, CurrentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UUndercoreAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UUndercoreAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UUndercoreAttributeSet, DefensePower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UUndercoreAttributeSet, DamageTaken);

};
