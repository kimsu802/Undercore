// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UndercoreLoadingScreenSettings.generated.h"


class UUserWidget;

/**
 * 
 */
UCLASS(Config = Game,DefaultConfig)
class UNDERCORE_API UUndercoreLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	TSubclassOf<UUserWidget>	GetLoadingScreenWidgetClassChecked() const;

public:
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetclass;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = false;
};
