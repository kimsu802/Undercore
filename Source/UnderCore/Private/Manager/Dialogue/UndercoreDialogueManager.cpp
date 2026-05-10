// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Dialogue/UndercoreDialogueManager.h"
#include "DataTable/UndercoreDialogueDataTable.h"
#include "Engine/AssetManager.h"
#include "Settings/MainMenuDeveloperSettings.h"
#include "UndercoreDebugHelper.h"
#include "Manager/MainMenuUISubsystem.h"
#include "UndercoreGameplayTags.h"
#include "UI/Widget_ActivatableBase.h"


UUndercoreDialogueManager* UUndercoreDialogueManager::Get(const UObject* WorldObjectContext)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldObjectContext, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UUndercoreDialogueManager>(World->GetGameInstance());
	}
	return nullptr;
}

void UUndercoreDialogueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	if (const UMainMenuDeveloperSettings* Settings = GetDefault<UMainMenuDeveloperSettings>())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			Settings->DialogueDataTable.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[this,Settings]()
				{
					CachedQuestDataTable = Settings->DialogueDataTable.Get();

					if (CachedQuestDataTable)
					{
						for (const auto& p : CachedQuestDataTable->GetRowMap())
						{
							FUndercoreDialogueRow* DialogueRow = (FUndercoreDialogueRow*)p.Value;

							if (DialogueRow)
							{
								FDialogueData Data;
								Data.ID = DialogueRow->ID;
								Data.DialogueLines = DialogueRow->DialogueLines;
								Data.bOnFinishedBlend = DialogueRow->bOnFinishedBlend;

								DialogueMap.Add(DialogueRow->ID, Data);
								Debug::Print(TEXT("Dialogue DataTable Completed !!"));
							}
						}
					}
				}	
			));

		/*UDataTable* LoadedTable = Settings->DialogueDataTable.LoadSynchronous();
		
		if (LoadedTable)
		{
			for (const auto& p : CachedQuestDataTable->GetRowMap())
			{
				FUndercoreDialogueRow* DialogueRow = (FUndercoreDialogueRow*)p.Value;

				if (DialogueRow)
				{
					FDialogueData Data;
					Data.ID = DialogueRow->ID;
					Data.DialogueLines = DialogueRow->DialogueLines;
					Data.bOnFinishedBlend = DialogueRow->bOnFinishedBlend;

					DialogueMap.Add(DialogueRow->ID, Data);
					Debug::Print(TEXT("Dialogue DataTable Completed !!"));
				}
			}
			
		}*/
	}


	
}

void UUndercoreDialogueManager::StartDialogue(FName InDialogueID)
{
	if (DialogueMap.Contains(InDialogueID))
	{
		StartDialogue(DialogueMap[InDialogueID]);
	}
}

void UUndercoreDialogueManager::StartDialogue(FText InText)
{
	if (!InText.IsEmpty())
	{
		StartDialogue(FDialogueData(InText));
	}
}

void UUndercoreDialogueManager::StartDialogue(const FDialogueData& InDialogueData)
{
	if (const UMainMenuDeveloperSettings* Settings = GetDefault<UMainMenuDeveloperSettings>())
	{
		if (!Settings->DialogueWidget.IsNull())
		{
			UMainMenuUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
				UndercoreGameplayTags::FrontEnd_WidgetStack_GameHud,
				Settings->DialogueWidget,
				[this,InDialogueData](EAsyncPushWidgetState State, UWidget_ActivatableBase* CreatedWidget)
				{
					if (State == EAsyncPushWidgetState::AfterPush)
					{
						if (CreatedWidget)
						{
							CurrentDialogueID = InDialogueData.ID;
							CurrentLineNumber = 0;
							bIsDialogueActive = true;


							CurrentDialogueLines =
								DialogueMap.Contains(CurrentDialogueID) ?
								DialogueMap[CurrentDialogueID].DialogueLines :
								InDialogueData.DialogueLines;

							if (!InDialogueData.DialogueLines.IsEmpty())
							{
								OnDialogueLineChanged.Broadcast(CurrentDialogueLines[CurrentLineNumber]);
							}
						}
					}

				});
		}
	}
}

void UUndercoreDialogueManager::NextDialogueLine()
{
	if (!CurrentDialogueLines.IsEmpty())
	{
		if (++CurrentLineNumber >= CurrentDialogueLines.Num())
		{
			const FDialogueData& CurrentDialogueData = 
				DialogueMap.Contains(CurrentDialogueID) ?
				DialogueMap.FindRef(CurrentDialogueID) :
				FDialogueData();


			OnDialogueFinished.Broadcast(CurrentDialogueData);
			bIsDialogueActive = false;
			return;	
		}

		OnDialogueLineChanged.Broadcast(CurrentDialogueLines[CurrentLineNumber]);
	}
}
