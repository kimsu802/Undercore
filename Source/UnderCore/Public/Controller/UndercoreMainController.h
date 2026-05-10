// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "UndercoreMainController.generated.h"


struct FDialogueLine;
struct FDialogueData;

/**
 * 
 */
UCLASS()
class UNDERCORE_API AUndercoreMainController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AUndercoreMainController();

	virtual FGenericTeamId GetGenericTeamId() const override;


protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	void OnDialogueLineChanged(const FDialogueLine& InDialogueLine);
	void OnDialogueFinished(const FDialogueData& InDialogueData);

private:
	FGenericTeamId MainTeamID;

	FName CurrentCameraTag = FName("");
};
