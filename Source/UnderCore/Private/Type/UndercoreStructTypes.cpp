// Fill out your copyright notice in the Description page of Project Settings.


#include "Type/UndercoreStructTypes.h"

FDialogueData::FDialogueData()
{
}

FDialogueData::FDialogueData(const FText& InText)
{
	FDialogueLine Data;
	Data.DialogueText = InText;
	DialogueLines.Add(Data);
}
