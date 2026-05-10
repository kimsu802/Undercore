// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget_ActivatableBase.h"
#include "Widget_ActivatablePush.generated.h"


class UCommonTextBlock;


/**
 * 
 */
UCLASS()
class UNDERCORE_API UWidget_ActivatablePush : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetPushText(FText InText);

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	PushCommonTextBlock;
};
