// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UndercoreLocalizationManager.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreLocalizationManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UUndercoreLocalizationManager* Get(const UObject* WorldContextObject);



};
