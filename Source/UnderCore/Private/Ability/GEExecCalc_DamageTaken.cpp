// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GEExecCalc_DamageTaken.h"
#include "Ability/UndercoreAttributeSet.h"
#include "UndercoreGameplayTags.h"


struct FUndercoreDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FUndercoreDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UUndercoreAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UUndercoreAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UUndercoreAttributeSet, DamageTaken, Target, false);
	}
};

static const FUndercoreDamageCapture& GetUndercoreDamageCapture()
{
	static FUndercoreDamageCapture UndercoreDamageCapture;

	return UndercoreDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetUndercoreDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetUndercoreDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetUndercoreDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();


	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetUndercoreDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(UndercoreGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(UndercoreGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(UndercoreGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetUndercoreDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = (UsedHeavyAttackComboCount + 1) * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
	}
	else if (UsedHeavyAttackComboCount == 0)
	{
		BaseDamage *= 1.15f;
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetUndercoreDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			));
	}
}
