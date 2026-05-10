// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "UndercoreInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<typename UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InputDataAsset, const FGameplayTag& InInputTag, ETriggerEvent TriggetEvent, UserObject* ContextObject, CallbackFunc Func);

	template<typename UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InputDataAsset, UserObject* ContextObject, CallbackFunc PressedFunc, CallbackFunc ReleasedFunc);
};

template<typename UserObject, typename CallbackFunc>
inline void UUndercoreInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InputDataAsset, const FGameplayTag& InInputTag, ETriggerEvent TriggetEvent, UserObject* ContextObject, CallbackFunc Func)
{
	if (InputDataAsset)
	{
		if (UInputAction* InputAction = InputDataAsset->FindNativeInputActionByTag(InInputTag))
		{
			BindAction(InputAction, TriggetEvent, ContextObject, Func);
		}
	}
}

template<typename UserObject, typename CallbackFunc>
inline void UUndercoreInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InputDataAsset, UserObject* ContextObject, CallbackFunc PressedFunc, CallbackFunc ReleasedFunc)
{
	if (InputDataAsset)
	{
		for (const FUndercoreInputActionConfig& AbilityInputActionConfig : InputDataAsset->AbilityInputActions)
		{
			BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, PressedFunc, AbilityInputActionConfig.InputTag);
			BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, ReleasedFunc, AbilityInputActionConfig.InputTag);
		}
	}
}
