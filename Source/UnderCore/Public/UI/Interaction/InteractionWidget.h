// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;
class UWidgetAnimation;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputUserSettings;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	InteractionCommonTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage>	InteractionCommonLazyImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>	InteractionWidgetAnimation;


public:
	//virtual void NativeConstruct() override;

	//void SetInteractionText(FText InText);
	void UpdateInteractionWidgetInfo(const FName& InMappingKey, const FText& InText = FText::GetEmpty());

private:
	FSlateBrush GetIconFromCurrentKey(const FKey& InKey) const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> CachedEILocalPlayerSubsystem;

	UPROPERTY(Transient)
	TObjectPtr<UEnhancedInputUserSettings> CachedOwningInputUserSettings;
};
