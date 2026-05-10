// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FUndercoreInputActionConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;

	bool IsValid() const
	{
		return InputAction && InputTag.IsValid();
	}
};



/**
 * 
 */
UCLASS()
class UNDERCORE_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext>	DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag", AllowPrivateAccess = true))
	TArray<FUndercoreInputActionConfig> NativeInputActions;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FUndercoreInputActionConfig> AbilityInputActions;

public:
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

	FORCEINLINE UInputMappingContext* GetDefaultMappingContext() { return DefaultMappingContext; }
	
};
