// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Type/UndercoreStructTypes.h"
#include "UndercoreDialogueManager.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueLineChangedDelegate, const FDialogueLine&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueFinishedDelegate, const FDialogueData&);

struct FUndercoreDialogueRow;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreDialogueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UUndercoreDialogueManager* Get(const UObject* WorldObjectContext);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void StartDialogue(FName InDialogueID);

	UFUNCTION(BlueprintCallable)
	void StartDialogue(FText InText);
	void StartDialogue(const FDialogueData& InDialogueData);

	void NextDialogueLine();

	FORCEINLINE bool GetbIsDialogueActive() const { return bIsDialogueActive;  }
public:
	FOnDialogueLineChangedDelegate OnDialogueLineChanged;
	FOnDialogueFinishedDelegate OnDialogueFinished;

private:
	UPROPERTY(Transient)
	TMap<FName, FDialogueData>		DialogueMap;

	TArray<FDialogueLine>			CurrentDialogueLines;

	int32 CurrentLineNumber = 0;

	FName CurrentDialogueID;

	bool bIsDialogueActive = false;

	TObjectPtr<UDataTable> CachedQuestDataTable = nullptr;
};
