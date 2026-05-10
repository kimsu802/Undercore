// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MainMenuCommonButtonBase.generated.h"


class UCommonTextBlock;
class UCommonLazyImage;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API UMainMenuCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);

protected:
	virtual void NativePreConstruct() override;

private:
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UCommonLazyImage> CommonLazyImage_ButtonImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta = (AllowPrivateAccess = true))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta = (AllowPrivateAccess = true))
	bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontEnd Button", meta = (AllowPrivateAccess = true))
	FText ButtonDescriptionText;
};
