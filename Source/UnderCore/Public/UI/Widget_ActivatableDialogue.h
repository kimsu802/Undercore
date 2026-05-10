// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget_ActivatableBase.h"
#include "Widget_ActivatableDialogue.generated.h"

class UCommonTextBlock;
struct FDialogueData;
struct FTimerHandle;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UWidget_ActivatableDialogue : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;


private:	
	UFUNCTION()
	 void OnDialogueLineChanged(const FDialogueLine& InDialogueLineData);

	 UFUNCTION()
	 void OnDialogueFinished(const FDialogueData& InDialogueData);

	 void OnNextBoundActionTriggered();

	 void TypeNextCharacter();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>	SpeakerCommonText;

	UPROPERTY(BlueprintReadOnly, meta = (BIndWidgetOptional))
	TObjectPtr<UCommonTextBlock>	DialogueCommonText;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Properties", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle NextAction;

	FUIActionBindingHandle	NextActionHandle;

	FString CurrentText;

	FString FullText;

	int32 CurrentIndex;

	FTimerHandle TypingTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float TypingSpeed = 0.2f;
};
