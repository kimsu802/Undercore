// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Type/UndercoreEnumTypes.h"
#include "UndercoreQuestManager.generated.h"

USTRUCT(BlueprintType)
struct FQuestStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StepIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestConditionType ConditionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> TargetEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetEnemiesNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSpawnEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AcceptRadius;

	int32 CurrentNum = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStepChanged, const FQuestStep&, NewStep);


struct FDialogueData;
class AInteractableActor;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static UUndercoreQuestManager* Get(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	void OnEnemyDead(AActor* DeadEnemy);

	UFUNCTION()
	void OnPlayerEnterTrigger(FName TriggerID);

	UFUNCTION(BlueprintCallable)
	void OnInteractActor(FName InTableID);

	UFUNCTION(BlueprintCallable)
	void SetCurrentQuestSteps(TArray<FQuestStep> InSteps) { CurrentSteps = InSteps; }

	UFUNCTION(BlueprintCallable)
	const TArray<FQuestStep>& GetCurrentQuestSteps() { return CurrentSteps; }

	FORCEINLINE int GetCurrentStepIndex() { return CurrentStepIndex;  }

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UFUNCTION(BlueprintCallable, Category = "Undercore|Quest")
	void StartQuest(FName InQuestID);
	void StartQuest(const TArray<FQuestStep>& InSteps);

	UFUNCTION(BlueprintCallable, Category = "Undercore|Quest")
	void StartStep(int index);
	void CompleteStep();
	void SpawnLocationToTrigger(const FQuestStep& InStep);
	void HandleSpawn();

	void OnDialogueFinished(const FDialogueData& InDialogueData);

	const FQuestStep& GetCurrentStep();

public:

	//UI 등등 바인딩 시키기 위한
	UPROPERTY(BlueprintAssignable)
	FOnQuestStepChanged	OnStepChanged;

private:
	int CurrentStepIndex = -1;

	TArray<FQuestStep> CurrentSteps;

	TWeakObjectPtr<AActor> SpawnedEnemy = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> CachedQuestDataTable = nullptr;
};
