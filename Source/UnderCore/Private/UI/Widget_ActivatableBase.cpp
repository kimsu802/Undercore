// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_ActivatableBase.h"
#include "Controller/MainMenuPlayerController.h"

AMainMenuPlayerController* UWidget_ActivatableBase::GetOwningMainMenuPlayerController()
{
	if (!CachedOwningMainMenuPlayerController.IsValid())
	{
		CachedOwningMainMenuPlayerController = GetOwningPlayer<AMainMenuPlayerController>();
	}

	return CachedOwningMainMenuPlayerController.IsValid() ? CachedOwningMainMenuPlayerController.Get() : nullptr;
}
