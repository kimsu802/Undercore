// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_ActivatableDialogue.h"
#include "Manager/Dialogue/UndercoreDialogueManager.h"
#include "CommonTextBlock.h"
#include "Type/UndercoreStructTypes.h"
#include "PlayerState/UndercorePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Input/CommonUIInputTypes.h"

void UWidget_ActivatableDialogue::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!NextAction.IsNull())
	{
		NextActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				NextAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnNextBoundActionTriggered)
			));
	}

	UUndercoreDialogueManager::Get(this)->OnDialogueLineChanged.AddUObject(this, &UWidget_ActivatableDialogue::OnDialogueLineChanged);
	UUndercoreDialogueManager::Get(this)->OnDialogueFinished.AddUObject(this, &UWidget_ActivatableDialogue::OnDialogueFinished);
}

void UWidget_ActivatableDialogue::NativeOnActivated()
{
	Super::NativeOnActivated();

	AddActionBinding(NextActionHandle);
}

void UWidget_ActivatableDialogue::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UWidget_ActivatableDialogue::OnDialogueFinished(const FDialogueData& InDialogueData)
{
	if (GetWorld())
	{
		CurrentText.Empty();
		FullText.Empty();
		CurrentIndex = -1;
		GetWorld()->GetTimerManager().ClearTimer(TypingTimer);
		DeactivateWidget();
	}
}

void UWidget_ActivatableDialogue::OnNextBoundActionTriggered()
{
	if (!FullText.IsEmpty())
	{
		if (CurrentIndex < FullText.Len())
		{
	
			if (DialogueCommonText)
			{
				GetWorld()->GetTimerManager().ClearTimer(TypingTimer);
				CurrentIndex = FullText.Len();
				CurrentText = FullText;
				DialogueCommonText->SetText(FText::FromString(CurrentText));
				return;
			}
		}

		UUndercoreDialogueManager::Get(this)->NextDialogueLine();
	}
}

void UWidget_ActivatableDialogue::TypeNextCharacter()
{
	if (CurrentIndex >= FullText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimer);
		return;
	}

	CurrentText.AppendChar(FullText[CurrentIndex]);
	CurrentIndex++;

	if (DialogueCommonText)
	{
		DialogueCommonText->SetText(FText::FromString(CurrentText));
	}
}

void UWidget_ActivatableDialogue::OnDialogueLineChanged(const FDialogueLine& InDialogueLineData)
{
	if (SpeakerCommonText)
	{
		if (InDialogueLineData.Speaker.IsEmpty())
		{
			if (APlayerState* State = UGameplayStatics::GetPlayerState(this,0))
			{
				if (AUndercorePlayerState* PlayerState = Cast<AUndercorePlayerState>(State))
				{
					SpeakerCommonText->SetText(FText::FromString(PlayerState->GetNickName()));
				}
			}
		}
		else
		{
			SpeakerCommonText->SetText(InDialogueLineData.Speaker);
		}
	}

	FullText = InDialogueLineData.DialogueText.ToString();
	CurrentText = TEXT("");
	CurrentIndex = 0;

	if (DialogueCommonText)
	{
		DialogueCommonText->SetText(FText::GetEmpty());
	}

	// 기존 타이머 제거
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimer);

		GetWorld()->GetTimerManager().SetTimer(
			TypingTimer,
			this,
			&UWidget_ActivatableDialogue::TypeNextCharacter,
			TypingSpeed,
			true
		);
	}
}
