// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_ActivatablePush.h"
#include "CommonTextBlock.h"

void UWidget_ActivatablePush::SetPushText(FText InText)
{
	check(PushCommonTextBlock);

	PushCommonTextBlock->SetText(InText);
}
