// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/UndercoreLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UUndercoreLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	check(!SoftLoadingScreenWidgetclass.IsNull());
	
	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetclass.LoadSynchronous();

	return	LoadedLoadingScreenWidget;
}
