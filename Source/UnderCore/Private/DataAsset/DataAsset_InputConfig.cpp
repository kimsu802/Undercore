// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	if (!NativeInputActions.IsEmpty())
	{
		for (const auto& p : NativeInputActions)
		{
			if (p.InputTag == InInputTag && p.InputAction)
			{
				return p.InputAction;
			}
		}
	}

	return nullptr;
}
