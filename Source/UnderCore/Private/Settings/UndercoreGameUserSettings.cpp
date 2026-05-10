// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/UndercoreGameUserSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Internationalization/StringTableRegistry.h"

UUndercoreGameUserSettings::UUndercoreGameUserSettings()
	:OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f),bAllowBackgroundAudio(false),bUseHDRAudioMode(false)
{
	RefreshTextStringTable();

	IConsoleManager::Get().RegisterConsoleCommand(TEXT("RefreshingTextStringData"), TEXT("Refresh the TextStringData"),
		FConsoleCommandDelegate::CreateLambda([]()
			{
				if (UUndercoreGameUserSettings* Settings = UUndercoreGameUserSettings::Get())
				{
					Settings->RefreshTextStringTable();
				}
			}));
}

UUndercoreGameUserSettings* UUndercoreGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UUndercoreGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UUndercoreGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
}

void UUndercoreGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

void UUndercoreGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;
}

void UUndercoreGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void UUndercoreGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
}

void UUndercoreGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}

float UUndercoreGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.f;
}

void UUndercoreGameUserSettings::SetCurrentLanguageSetting(const FString& InNewLanguage)
{
	CurrentLanguageSetting = InNewLanguage;
}

FString UUndercoreGameUserSettings::GetCurrentLanguageSetting() const
{
	const FString& DefaultCurrentLanguage = UKismetInternationalizationLibrary::GetCurrentLanguage();
	return CurrentLanguageSetting.IsEmpty() ? DefaultCurrentLanguage : CurrentLanguageSetting;
}

void UUndercoreGameUserSettings::SetAllowedAutoAim(bool bAllowed)
{
	bIsAllowedAutoAim = bAllowed;
}

void UUndercoreGameUserSettings::RefreshTextStringTable()
{
	UKismetInternationalizationLibrary::SetCurrentLanguage(GetCurrentLanguageSetting());

	const FString language = UKismetInternationalizationLibrary::GetCurrentLanguage();

	const FString RelativePath = FString::Printf(TEXT("Localization/%s/textstringdata.csv"), *language);
	const FString FullPath = FPaths::ProjectContentDir() + RelativePath;
	const FString NormalizedPath = FPaths::ConvertRelativePathToFull(FullPath);

	UE_LOG(LogTemp, Warning, TEXT("StringTable Path: %s"), *NormalizedPath);
	UE_LOG(LogTemp, Warning, TEXT("File Exists: %s"), IFileManager::Get().FileExists(*NormalizedPath) ? TEXT("YES") : TEXT("NO"));

	FStringTableRegistry::Get().UnregisterStringTable("GameText");

	LOCTABLE_FROMFILE_GAME("GameText", "GameTextNameSpace", "Localization/" + language + "/textstringdata.csv");
}