// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObject/ObtionsDataRegistry.h"
#include "UI/DataObject/ListDataObject_String.h"
#include "UI/DataObject/ListDataObject_Collection.h"
#include "UI/DataObject/ListDataObject_Scalar.h"
#include "UI/DataObject/ListDataObject_StringResolution.h"
#include "UI/DataObject/ListDataObject_KeyRemap.h"
#include "Settings/UndercoreGameUserSettings.h"
#include "Type/UndercoreStructTypes.h"
#include "UndercoreFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "UndercoreDebugHelper.h"




#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UUndercoreGameUserSettings,SetterOrGetterFuncName))

void UObtionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwningLocalPlayer);
}

TArray<UListDataObject_Base*> UObtionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UObtionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UObtionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData("gameplay_tap_title"));

	//TSharedPtr< FOptionsDataInteractionHelper> ConstructHelper = 
	//	MakeShared<FOptionsDataInteractionHelper>(
	//		GET_FUNCTION_NAME_STRING_CHECKED(UUndercoreGameUserSettings, GetCurrentGameDifficulty)
	//	);
		
	//난이도
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameplayTabCollection"));
		GameDifficulty->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_title"));
		GameDifficulty->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_description"));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_easy"));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_normal"));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_hard"));
		GameDifficulty->AddDynamicOption(TEXT("Hell"), UUndercoreFunctionLibrary::GetTextFromStringData("gamedifficulty_hell"));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	//자동 조준 허용 여부
	{
		UListDataObject_StringBool* AllowAutoAim = NewObject<UListDataObject_StringBool>();
		AllowAutoAim->SetDataID(FName("AllowBackgroundAudio"));
		AllowAutoAim->SetDataDisplayName(FText::FromString(TEXT("자동 조준 보정")));
		AllowAutoAim->OverrideTrueDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_enabled")));
		AllowAutoAim->OverrideFalseDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_disabled")));
		AllowAutoAim->SetFalseAsDefaultValue();
		AllowAutoAim->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowedAutoAim));
		AllowAutoAim->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowedAutoAim));
		AllowAutoAim->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(AllowAutoAim);
	}

	UListDataObject_Collection* LocalizationCategory = NewObject<UListDataObject_Collection>();
	LocalizationCategory->SetDataID(FName("LocalizationCategory"));
	LocalizationCategory->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("localization_category_title")));

	GameplayTabCollection->AddChildListData(LocalizationCategory);

	//언어
	{
		UListDataObject_String* LanguageSetting = NewObject<UListDataObject_String>();
		LanguageSetting->SetDataID(FName("LanguageSetting"));
		LanguageSetting->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("language_title")));
		LanguageSetting->AddDynamicOption(TEXT("ko"), UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("language_kr")));
		LanguageSetting->AddDynamicOption(TEXT("en"), UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("language_en")));
		LanguageSetting->SetDefaultValueFromString(UUndercoreGameUserSettings::Get()->GetCurrentLanguageSetting());
		LanguageSetting->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentLanguageSetting));
		LanguageSetting->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentLanguageSetting));

		LocalizationCategory->AddChildListData(LanguageSetting);
	}


	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UObtionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("audio_tab_title")));

	//볼륨 카테고리
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("volume_category_title")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		//전체 음량
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("overallvolume_title")));
			OverallVolume->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("overallvolume_description")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);
			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		//음악 볼륨
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("musicvolume_title")));
			MusicVolume->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("overallvolume_description")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplySettingsImmediately(true);
			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		//효과음 볼륨
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("soundfxvolume_title")));
			SoundFXVolume->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("soundfxvolume_description")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplySettingsImmediately(true);
			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}

	}

	//사운드 카테고리
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("sound_category_title")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// 배경음 허용 여부
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("backgroundaudio_title")));
			AllowBackgroundAudio->OverrideTrueDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("hdraudio_title")));
			UseHDRAudioMode->OverrideTrueDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_disabled")));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}


	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UObtionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("video_tap_title")));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//비디오 카테고리
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("display_category_title")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);

		PackagedBuildOnlyCondition.SetDisabledRichReason(UUndercoreFunctionLibrary::GetStringFromStringData("package_disabled_reason"));


		//윈도우 모드
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("windowmode_title")));
			WindowMode->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("windowmode_description")));
			WindowMode->AddEnumOptions(EWindowMode::Fullscreen, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_fullscreen")));
			WindowMode->AddEnumOptions(EWindowMode::WindowedFullscreen, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_windowedfullscreen")));
			WindowMode->AddEnumOptions(EWindowMode::Windowed, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_windowed")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		//해상도
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("screenresolution_title")));
			ScreenResolution->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("screenresolution_description")));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					const bool bIsBorderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBorderlessWindow;
				});


			WindowModeEditCondition.SetDisabledRichReason(UUndercoreFunctionLibrary::GetStringFromStringData(TEXT("windowmode_disabled_reason")));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);

		}
	}

	//그래픽 카테고리
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("graphics_category_title")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//디스플레이 밝기
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("displaygamma_title")));
			DisplayGamma->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("displaygamma_description")));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

		//전체 품질
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("overallquality_title")));
			OverallQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("overallquality_description")));
			OverallQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			OverallQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			OverallQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			OverallQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			OverallQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		//해상도 품질
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("resolutionscale_title")));
			ResolutionScale->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("resolutionscale_description")));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.1f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//라이팅 품질
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("globalilluminationquality_title")));
			GlobalIlluminationQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("globalilluminationquality_description")));
			GlobalIlluminationQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			GlobalIlluminationQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			GlobalIlluminationQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			GlobalIlluminationQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			GlobalIlluminationQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		//그림자 품질
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("shadowquality_title")));
			ShadowQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("shadowquality_description")));
			ShadowQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			ShadowQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			ShadowQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			ShadowQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			ShadowQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		//안티앨리어싱 품질
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("antialiasingquality_title")));
			AntiAliasingQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("antialiasingquality_description")));
			AntiAliasingQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			AntiAliasingQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			AntiAliasingQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			AntiAliasingQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			AntiAliasingQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//시야 품질
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("viewdistance_title")));
			ViewDistanceQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("viewdistance_description")));
			ViewDistanceQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_near")));
			ViewDistanceQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_medium")));
			ViewDistanceQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_far")));
			ViewDistanceQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryfar")));
			ViewDistanceQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//텍스처 품질
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("texturequality_title")));
			TextureQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("texturequality_description")));
			TextureQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			TextureQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			TextureQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			TextureQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			TextureQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//시각 효과 품질
		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("visualeffectquality_title")));
			VisualEffectQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("visualeffectquality_description")));
			VisualEffectQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			VisualEffectQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			VisualEffectQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			VisualEffectQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			VisualEffectQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		//반사 품질
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("reflectionquality_title")));
			ReflectionQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("reflectionquality_description")));
			ReflectionQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			ReflectionQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			ReflectionQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			ReflectionQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			ReflectionQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		//포스트 프로세싱
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("postprocessingquality_title")));
			PostProcessingQuality->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("postprocessingquality_description")));
			PostProcessingQuality->AddIntegerOption(0, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_low")));
			PostProcessingQuality->AddIntegerOption(1, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_normal")));
			PostProcessingQuality->AddIntegerOption(2, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_high")));
			PostProcessingQuality->AddIntegerOption(3, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_veryhigh")));
			PostProcessingQuality->AddIntegerOption(4, UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_cinematic")));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	//기타 카테고리
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("advancedgraphics_category_title")));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//수직 동기화
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("verticalsync_title")));
			VerticalSync->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("verticalsync_description")));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);

			FullScreenOnlyCondition.SetDisabledRichReason(UUndercoreFunctionLibrary::GetStringFromStringData(TEXT("fullscreenonly_disabled_reason")));
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));
			
			VerticalSync->AddEditCondition(FullScreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//프레임 제한
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName(""));
			FrameRateLimit->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("frameratelimit_title")));
			FrameRateLimit->SetDescriptionRichText(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("frameratelimit_description")));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f),FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("string_nolimit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}

	}

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UObtionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("control_tab_title")));

	UEnhancedInputLocalPlayerSubsystem* EISubSystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubSystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubSystem->GetUserSettings();

	check(EIUserSettings);

	//키보드와 마우스 카테고리
	{
		UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("ControlTabCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(UUndercoreFunctionLibrary::GetTextFromStringData(TEXT("keyboard_mouse_category_title")));

		//키보드 마우스 입력
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;

			//FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			//KeyboardMouseOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
			//KeyboardMouseOnly.bMatchBasicKeyTypes = true;

			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							//Debug::Print(
							//	TEXT(" Mapping ID : ") + KeyMapping.GetMappingName().ToString() +
							//	TEXT(" Display Name : ") + KeyMapping.GetDisplayName().ToString() +
							//	TEXT(" Bound Key : ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
							//);

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}

		ControlTabCollection->AddChildListData(KeyboardMouseCategoryCollection);
	}

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
