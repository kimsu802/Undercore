// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UndercoreBossBarWidget.h"
#include "Character/UndercoreEnemyCharacter.h"
#include "Ability/UndercoreAttributeSet.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "UndercoreFunctionLibrary.h"
#include "GameplayEffectTypes.h"
#include "CommonTextBlock.h"

void UUndercoreBossBarWidget::Init(AUndercoreEnemyCharacter* InCharacter)
{
	if (UUndercoreAbilitySystemComponent* ASC = UUndercoreFunctionLibrary::GetUndercoreASCFromActor(InCharacter))
	{
		CachedMaxHealth = ASC->GetNumericAttributeChecked(UUndercoreAttributeSet::GetMaxHealthAttribute());
		CachedCurrentHealth = ASC->GetNumericAttributeChecked(UUndercoreAttributeSet::GetCurrentHealthAttribute());

		ASC->GetGameplayAttributeValueChangeDelegate(UUndercoreAttributeSet::GetCurrentHealthAttribute()).AddUObject(this, &UUndercoreBossBarWidget::OnCurrentHealthChanged);

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				HealthTimer,
				FTimerDelegate::CreateUObject(this, &UUndercoreBossBarWidget::StartHealthToMaxHealth, HealthIncreaseSpeed),
				0.15f,
				true
			);
		}		
	}

	if (BossNameCommonTextBlock)
	{
		BossNameCommonTextBlock->SetText(InCharacter->GetDisplayName());
	}
}

void UUndercoreBossBarWidget::NativeDestruct()
{
	if (CachedASC)
	{
		CachedASC->GetGameplayAttributeValueChangeDelegate(UUndercoreAttributeSet::GetCurrentHealthAttribute()).RemoveAll(this);
	}


	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
	}
}

void UUndercoreBossBarWidget::OnCurrentHealthChanged(const FOnAttributeChangeData& InChangedData)
{
	CachedCurrentHealth = InChangedData.NewValue;

	if (!GetWorld()->GetTimerManager().IsTimerActive(HealthTimer))
	{	
		UpdateProgressBar(CachedCurrentHealth / CachedMaxHealth);

		if (CachedCurrentHealth / CachedMaxHealth <= 0.f)
		{
			RemoveFromParent();
		}
	}
}

void UUndercoreBossBarWidget::StartHealthToMaxHealth(float InSpeed)
{
	if (CurrentPercentage >= (CachedCurrentHealth / CachedMaxHealth))
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
		}

		return;
	}

	CurrentPercentage += InSpeed;

	UpdateProgressBar(CurrentPercentage);
}
