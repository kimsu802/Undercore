// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/UndercoreAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "UndercoreGameplayTags.h"
#include "UndercoreFunctionLibrary.h"


UUndercoreAttributeSet::UUndercoreAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UUndercoreAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		// 캐릭터 사망 처리
		if (GetCurrentHealth() == 0.f)
		{
			UUndercoreFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), UndercoreGameplayTags::Shared_Status_Dead);
		}
	}
}
