// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UndercoreBossBarWidget.generated.h"

class	AUndercoreEnemyCharacter;
struct	FOnAttributeChangeData;
class	UUndercoreAbilitySystemComponent;
class	UCommonTextBlock;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreBossBarWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(AUndercoreEnemyCharacter* InCharacter);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProgressBar(float InPercent);

private:
	virtual void	NativeDestruct() override;
	void			OnCurrentHealthChanged(const FOnAttributeChangeData& InChangedData);
	void			StartHealthToMaxHealth(float InSpeed);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	BossNameCommonTextBlock;


private:
	float CachedMaxHealth = 0.f;
	float CachedCurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float HealthIncreaseSpeed = 0.15f;

	float CurrentPercentage = 0.f;

	UPROPERTY()
	TObjectPtr<UUndercoreAbilitySystemComponent> CachedASC;

	FTimerHandle HealthTimer;
};
