// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AsyncAction_PushConfirmUI.h"
#include "Manager/MainMenuUISubsystem.h"

UAsyncAction_PushConfirmUI* UAsyncAction_PushConfirmUI::PushConfirmScreen(const UObject* WorldContextObject, EConfirmUIType UIType, FText InTitle, FText InMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushConfirmUI* Node = NewObject< UAsyncAction_PushConfirmUI>();
			Node->CachedOwningWorld = World;
			Node->CachedUIType = UIType;
			Node->CachedMessage = InMessage;
			Node->CachedTitle = InTitle;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	return nullptr;
}

void UAsyncAction_PushConfirmUI::Activate()
{
	if (CachedOwningWorld.IsValid())
	{
		UMainMenuUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(CachedUIType, CachedTitle, CachedMessage,
			[this](EConfirmButtonType ClickedButtonType)
			{
				OnButtonClicked.Broadcast(ClickedButtonType);

				SetReadyToDestroy();
			});
	}
}
