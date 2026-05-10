// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class AMainMenuPlayerController;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure)
	AMainMenuPlayerController* GetOwningMainMenuPlayerController();

private:
	TWeakObjectPtr<AMainMenuPlayerController> CachedOwningMainMenuPlayerController;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsInGame = false;
};
