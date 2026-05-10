// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "UI/Widget_ActivatableBase.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick), BlueprintType)
class UNDERCORE_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate	OnKeyRemapScreenKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectedCanceledDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenKeySelectedCanceledDelegate OnKeyRemapScreenKeySelectCanceled;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCanceled(const FString& CanceledReason);

	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonRichTextBlock>	CommonRichTextBlock_RemapMessage;

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreProcessor;

	ECommonInputType CachedDesiredInputType;
};
