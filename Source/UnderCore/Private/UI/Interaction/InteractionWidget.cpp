// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/InteractionWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "UndercoreDebugHelper.h"


void UInteractionWidget::UpdateInteractionWidgetInfo(const FName& InMappingKey, const FText& InText)
{
	if (!CachedEILocalPlayerSubsystem)
	{
		CachedEILocalPlayerSubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	
	check(CachedEILocalPlayerSubsystem);

	if (!CachedOwningInputUserSettings)
	{
		CachedOwningInputUserSettings = CachedEILocalPlayerSubsystem->GetUserSettings();
	}

	check(CachedOwningInputUserSettings);

	FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
	KeyboardMouseOnly.KeyToMatch = EKeys::S;
	KeyboardMouseOnly.bMatchBasicKeyTypes = true;

	//FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
	//KeyboardMouseOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom;
	//KeyboardMouseOnly.bMatchBasicKeyTypes = true;

	for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : CachedOwningInputUserSettings->GetAllSavedKeyProfiles())
	{
		UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

		check(MappableKeyProfile);

		for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
		{
			for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings)
			{
				if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
				{
					if (KeyMapping.GetMappingName() == InMappingKey)
					{
						InteractionCommonTextBlock->SetText(InText.IsEmpty() ? KeyMapping.GetDisplayName() : InText);
						InteractionCommonLazyImage->SetBrushFromTexture(Cast<UTexture2D>(GetIconFromCurrentKey(KeyMapping.GetCurrentKey()).GetResourceObject()));
						PlayAnimation(InteractionWidgetAnimation);
					}
					//Debug::Print(
					//	TEXT(" Mapping ID : ") + KeyMapping.GetMappingName().ToString() +
					//	TEXT(" Display Name : ") + KeyMapping.GetDisplayName().ToString() +
					//	TEXT(" Bound Key : ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
					//);
				}
			}
		}
	}
	
}

FSlateBrush UInteractionWidget::GetIconFromCurrentKey(const FKey& InKey) const
{
	check(CachedOwningInputUserSettings);

	FSlateBrush FoundBrush;

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedOwningInputUserSettings->GetLocalPlayer());

	check(CommonInputSubsystem);

	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush,
		InKey,
		ECommonInputType::MouseAndKeyboard,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	if (!bHasFoundBrush)
	{
		Debug::Print(
			TEXT("Unable to find an icon for the key") +
			InKey.GetDisplayName().ToString() +
			TEXT(" Empty brush was applied.")
		);
	}

	return FoundBrush;
}
