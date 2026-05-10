// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainMenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Settings/UndercoreGameUserSettings.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/KismetInternationalizationLibrary.h"


void AMainMenuPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("Default"), FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTarget(FoundCameras[0]);
	}

	if (UUndercoreGameUserSettings* GameUserSettings = UUndercoreGameUserSettings::Get())
	{
		if (GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
		{
			GameUserSettings->RunHardwareBenchmark();
			GameUserSettings->ApplyHardwareBenchmarkResults();
		}
		GameUserSettings->RefreshTextStringTable();
	}


}
