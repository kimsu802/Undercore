// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
};
