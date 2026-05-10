// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/UndercoreMainController.h"
#include "Manager/Dialogue/UndercoreDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/UndercoreGameUserSettings.h"

AUndercoreMainController::AUndercoreMainController()
{
    MainTeamID = FGenericTeamId(0);
}

FGenericTeamId AUndercoreMainController::GetGenericTeamId() const
{
    return MainTeamID;
}

void AUndercoreMainController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

    UUndercoreDialogueManager::Get(this)->OnDialogueLineChanged.AddUObject(this, &AUndercoreMainController::OnDialogueLineChanged);
    UUndercoreDialogueManager::Get(this)->OnDialogueFinished.AddUObject(this, &AUndercoreMainController::OnDialogueFinished);

    if (UUndercoreGameUserSettings* GameUserSettings = UUndercoreGameUserSettings::Get())
    {
        GameUserSettings->RefreshTextStringTable();
    }
}

void AUndercoreMainController::OnUnPossess()
{
    Super::OnUnPossess();

    UUndercoreDialogueManager::Get(this)->OnDialogueLineChanged.RemoveAll(this);
    UUndercoreDialogueManager::Get(this)->OnDialogueFinished.RemoveAll(this);

}

void AUndercoreMainController::OnDialogueLineChanged(const FDialogueLine& InDialogueLine)
{
    if (CurrentCameraTag == InDialogueLine.ViewCameraTag)
    {
        return;
    }

    if (InDialogueLine.ViewCameraTag == FName(""))
    {
        if (APawn* MyPawn = GetPawn())
        {
            CurrentCameraTag = FName("");
            SetViewTargetWithBlend(MyPawn);
            return;
        }
    }

    TArray<AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsOfClassWithTag(this, AActor::StaticClass(), InDialogueLine.ViewCameraTag, FoundCameras);

    if (!FoundCameras.IsEmpty())
    {
        SetViewTargetWithBlend(FoundCameras[0]);
        CurrentCameraTag = InDialogueLine.ViewCameraTag;
    }

}

void AUndercoreMainController::OnDialogueFinished(const FDialogueData& InDialogueData)
{
      if (APawn* MyPawn = GetPawn())
      {
          CurrentCameraTag = FName("");
          SetViewTargetWithBlend(MyPawn, InDialogueData.bOnFinishedBlend ? 0.5f : 0.0f);
      }
}
