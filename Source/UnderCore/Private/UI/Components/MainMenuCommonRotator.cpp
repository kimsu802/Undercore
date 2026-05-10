// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/MainMenuCommonRotator.h"
#include "CommonTextBlock.h"

void UMainMenuCommonRotator::SetSelectedOptionsByText(const FText& InTextOption)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InTextOption](const FText& TextItem)->bool
		{
			return TextItem.EqualTo(InTextOption);
		}
	);

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}

	else
	{
		MyText->SetText(InTextOption);
	}
}
