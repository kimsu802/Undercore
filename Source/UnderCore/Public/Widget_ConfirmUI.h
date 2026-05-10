// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget_ActivatableBase.h"
#include "Type/UndercoreEnumTypes.h"
#include "Widget_ConfirmUI.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

USTRUCT(BlueprintType)
struct FConfirmButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmButtonType ConfirmScreenButtonType = EConfirmButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class UNDERCORE_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg);

	UPROPERTY(Transient)
	FText Title;

	UPROPERTY(Transient)
	FText Message;

	UPROPERTY(Transient)
	TArray<FConfirmButtonInfo> AvailableButtons;

};



/**
 * 
 */
UCLASS(BlueprintType, Abstract, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_ConfirmUI : public UWidget_ActivatableBase
{
	GENERATED_BODY()
public:
	void InitConfirmUI(UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmButtonType)> ClickedButtonCallback);
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	CommonTextBlockTitle;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	CommonTextBlockMessage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UDynamicEntryBox>	DynamicEntryBoxButtons;
};	
