// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Quest/UndercoreQuestManager.h"
#include "Settings/MainMenuDeveloperSettings.h"
#include "UndercoreDebugHelper.h"
#include "Manager/Dialogue/UndercoreDialogueManager.h"
#include "DataTable/UndercoreQuestStepDataTable.h"
#include "Item/UndercoreQuestTrigger.h"
#include "Item/InteractableActor.h"
#include "Character/UndercoreEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UUndercoreQuestManager* UUndercoreQuestManager::Get(UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UUndercoreQuestManager>(World->GetGameInstance());
	}
	return nullptr;
}

void UUndercoreQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UMainMenuDeveloperSettings* MainMenuDeveloperSettings = GetDefault<UMainMenuDeveloperSettings>())
	{
		CachedQuestDataTable = MainMenuDeveloperSettings->QuestDataTable.LoadSynchronous();
	}

	Collection.InitializeDependency<UUndercoreDialogueManager>();

	UUndercoreDialogueManager::Get(this)->OnDialogueFinished.AddUObject(this,&UUndercoreQuestManager::OnDialogueFinished);
}

void UUndercoreQuestManager::Deinitialize()
{
	Super::Deinitialize();

	UUndercoreDialogueManager::Get(this)->OnDialogueFinished.RemoveAll(this);
}

void UUndercoreQuestManager::StartQuest(FName InQuestID)
{
	check(CachedQuestDataTable);

	CurrentSteps.Empty();

	for (const TPair<FName, uint8*>& p : CachedQuestDataTable->GetRowMap())
	{
		FUndercoreQuestStepRow* QuestStepRow = (FUndercoreQuestStepRow*)p.Value;

		if (QuestStepRow)
		{
			FQuestStep QuestStepData;
			QuestStepData.QuestID = QuestStepRow->QuestID;
			QuestStepData.StepIndex = QuestStepRow->StepIndex;
			QuestStepData.Description = QuestStepRow->Description;
			QuestStepData.ConditionType = QuestStepRow->ConditionType;
			QuestStepData.TargetLocation = QuestStepRow->TargetLocation;
			QuestStepData.TargetTag = QuestStepRow->TargetTag;
			QuestStepData.TargetEnemiesNum = QuestStepRow->TargetEnemiesNum;
			QuestStepData.AcceptRadius = QuestStepRow->AcceptRadius;
			QuestStepData.DialogueID = QuestStepRow->DialogueID;
			QuestStepData.bSpawnEnemy = QuestStepRow->bSpawnEnemy;
			QuestStepData.TargetEnemyClass = QuestStepRow->TargetEnemyClass;

			CurrentSteps.Add(QuestStepData);
		}
	}

	StartQuest(CurrentSteps);
	
}

void UUndercoreQuestManager::StartQuest(const TArray<FQuestStep>& InSteps)
{
	CurrentSteps = InSteps;
	CurrentStepIndex = -1;

	StartStep(0);
}

void UUndercoreQuestManager::StartStep(int index)
{
	if (!CurrentSteps.IsValidIndex(index))
	{
		Debug::Print(TEXT("Quest Finished !!"));

		return;
	}

	CurrentStepIndex = index;

	const FQuestStep* Step = CurrentSteps.FindByPredicate([index](const FQuestStep& InStep)
		{
			return InStep.StepIndex == index;
		});

	OnStepChanged.Broadcast(*Step);

	if (Step->ConditionType == EQuestConditionType::Location)
	{
		SpawnLocationToTrigger(*Step);
		return;
	}

	if (Step->bSpawnEnemy)
	{
		if (GetWorld())
		{
			if(APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0))
			{
				FVector Direction = (PlayerPawn->GetActorLocation() - Step->TargetLocation).GetSafeNormal();
				FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(Direction);

				GetWorld()->SpawnActor<AUndercoreEnemyCharacter>(
					Step->TargetEnemyClass,
					Step->TargetLocation,
					SpawnRotation);
			}
		}
	}

	if (!Step->DialogueID.IsNone())
	{
		UUndercoreDialogueManager::Get(this)->StartDialogue(Step->DialogueID);
	}
}

void UUndercoreQuestManager::CompleteStep()
{
	Debug::Print(TEXT("Step Completed !!"));

	StartStep(CurrentStepIndex + 1);
}

void UUndercoreQuestManager::SpawnLocationToTrigger(const FQuestStep& InStep)
{
	if (!GetWorld()) return;

	FActorSpawnParameters Params;

	AUndercoreQuestTrigger* Trigger = GetWorld()->SpawnActor<AUndercoreQuestTrigger>(
		AUndercoreQuestTrigger::StaticClass(),
		InStep.TargetLocation,
		FRotator::ZeroRotator,
		Params
	);

	if (Trigger)
	{
		Trigger->SetActorScale3D(FVector(1.f));
		Trigger->SetTriggerID(InStep.TargetTag);
	}
}

void UUndercoreQuestManager::OnPlayerEnterTrigger(FName TriggerID)
{
	if (!CurrentSteps.IsValidIndex(CurrentStepIndex)) 
		return;

	const FQuestStep& Step = CurrentSteps[CurrentStepIndex];

	if (Step.ConditionType != EQuestConditionType::Location)
		return;

	if (Step.TargetTag == TriggerID)
	{
		CompleteStep();
	}
}

void UUndercoreQuestManager::OnInteractActor(FName InTableID)
{
	if (CurrentSteps[CurrentStepIndex].ConditionType == EQuestConditionType::Interact)
	{
		if (InTableID == CurrentSteps[CurrentStepIndex].TargetTag)
		{
			CompleteStep();
		}
	}
}

void UUndercoreQuestManager::OnDialogueFinished(const FDialogueData& InDialogueData)
{
	if (!CurrentSteps.IsValidIndex(CurrentStepIndex)) 
		return;

	const FQuestStep& Step = CurrentSteps[CurrentStepIndex];

	if (Step.ConditionType != EQuestConditionType::Dialogue)
		return;

	if (Step.DialogueID == InDialogueData.ID)
	{
		CompleteStep();
	}
}

void UUndercoreQuestManager::OnEnemyDead(AActor* DeadEnemy)
{
	if (!CurrentSteps.IsValidIndex(CurrentStepIndex)) return;

	if (!CurrentSteps.IsEmpty())
	{
		FQuestStep& CurrentStep = CurrentSteps[CurrentStepIndex];

		if (CurrentStep.ConditionType == EQuestConditionType::Kill)
		{
			if (CurrentStep.TargetEnemyClass == DeadEnemy->GetClass())
			{
				if (++CurrentStep.CurrentNum >= CurrentStep.TargetEnemiesNum)
				{
					CompleteStep();
				}
			}
		}
	}
}

const FQuestStep& UUndercoreQuestManager::GetCurrentStep()
{
	return CurrentSteps[CurrentStepIndex];
}
