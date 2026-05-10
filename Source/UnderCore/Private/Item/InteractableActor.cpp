// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InteractableActor.h"
#include "Manager/MainMenuUISubsystem.h"
#include "DataTable/UndercoreItemDataTable.h"
#include "Manager/Quest/UndercoreQuestManager.h"
#include "Manager/Dialogue/UndercoreDialogueManager.h"
#include "Manager/UndercoreItemDataManager.h"
#include "Character/UndercoreMainCharacter.h"

void AInteractableActor::Interact(ACharacter* InCharacter)
{
	// 일반 상호작용 가능한 액터는 디폴트로 해당 액터의 정보에 대한 설명을 Widget으로 푸쉬한다.
	if (CanInteract())
	{
		if (const FUndercoreItemData* ItemData = UUndercoreItemDataManager::Get(this)->FindItemDataByID(TableID))
		{
			if (UUndercoreDialogueManager* DialogueManager = UUndercoreDialogueManager::Get(this))
			{
				FDialogueData DescData(FText::FromString(ItemData->Description_key));
				DialogueManager->StartDialogue(DescData);

				TSharedPtr<FDelegateHandle> Handle = MakeShared<FDelegateHandle>();

				*Handle = DialogueManager->OnDialogueFinished.AddLambda([this, InCharacter, DialogueManager, Handle](const FDialogueData& Data)
					{
						BP_Interact(InCharacter);
						
						DialogueManager->OnDialogueFinished.Remove(*Handle);
					});				
			}		
			return;
		}
		BP_Interact(InCharacter);
	}

	return;
}

bool AInteractableActor::CanInteract()
{
	return true;
}

FName AInteractableActor::GetBindingKeyNameByIndex(int index)
{
	if (!BindingKeyName.IsEmpty() && BindingKeyName.IsValidIndex(index))
	{
		return BindingKeyName[index];
	}

	return FName();
}
