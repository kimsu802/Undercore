// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UndercoreGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNDERCORE_API UUndercoreGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UUndercoreGameUserSettings();

	static UUndercoreGameUserSettings* Get();

	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }

	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSoundFXVolume() const { return SoundFXVolume; }

	UFUNCTION()
	void SetSoundFXVolume(float InVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio;  }

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode; }

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);

	UFUNCTION()
	float GetCurrentDisplayGamma() const;
	
	UFUNCTION()
	void SetCurrentLanguageSetting(const FString& InNewLanguage);

	UFUNCTION()
	FString GetCurrentLanguageSetting() const;

	UFUNCTION()
	void SetAllowedAutoAim(bool bAllowed);

	UFUNCTION(BlueprintPure)
	bool GetAllowedAutoAim() { return bIsAllowedAutoAim;  }

	UFUNCTION()
	void RefreshTextStringTable();

private:
	UPROPERTY(config)
	FString CurrentGameDifficulty;

	UPROPERTY(config)
	FString CurrentLanguageSetting;

	UPROPERTY(config)
	float OverallVolume;

	UPROPERTY(config)
	float MusicVolume;

	UPROPERTY(config)
	float SoundFXVolume;

	UPROPERTY(config)
	bool bAllowBackgroundAudio;

	UPROPERTY(config)
	bool bUseHDRAudioMode;

	UPROPERTY(config,BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsAllowedAutoAim;
};
