// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UndercoreStructTypes.generated.h"

/**
 * 
 */

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(TFunction<bool()> InEditConfitionFunc)
	{
		EditConditionFunc = InEditConfitionFunc;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}

		return true;
	}

	FString GetDisabledRichReason() const
	{
		return DisabledRichReason;
	}

	void SetDisabledRichReason(const FString& InRichReason)
	{
		DisabledRichReason = InRichReason;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InForcedValue)
	{
		DisabledForcedStringValue = InForcedValue;
	}

private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};

USTRUCT()
struct FBanWordNode
{
	GENERATED_BODY()

	TMap<TCHAR, FBanWordNode*> Children;

	FBanWordNode* Failure = nullptr;

	bool bIsEnd = false;
};

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ViewCameraTag;
};

USTRUCT(BlueprintType)
struct FDialogueData
{
	GENERATED_BODY()

public:
	FDialogueData();
	FDialogueData(const FText& InText);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueLine>	DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOnFinishedBlend = false;
};