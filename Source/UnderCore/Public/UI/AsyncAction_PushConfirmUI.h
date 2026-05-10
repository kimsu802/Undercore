// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Type/UndercoreEnumTypes.h"
#include "AsyncAction_PushConfirmUI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmUIButtonClickedDelegate, EConfirmButtonType, ClickedButtonType);


/**
 * 
 */
UCLASS()
class UNDERCORE_API UAsyncAction_PushConfirmUI : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation UI"))
	static UAsyncAction_PushConfirmUI* PushConfirmScreen(const UObject* WorldContextObject, EConfirmUIType UIType, FText InTitle, FText InMessage);

	virtual void Activate() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnConfirmUIButtonClickedDelegate OnButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmUIType CachedUIType;
	FText CachedTitle;
	FText CachedMessage;

};
