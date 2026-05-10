// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "Type/UndercoreEnumTypes.h"
#include "MainMenuUISubsystem.generated.h"


class UWidget_PrimaryLayout;
class UWidget_ActivatableBase;
class UWidget_ActivatablePush;

struct FPushWidgetRequest
{
	TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass;
	FText Message;
};

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UMainMenuCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

/**
 * 
 */
UCLASS()
class UNDERCORE_API UMainMenuUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UMainMenuUISubsystem* Get(const UObject* WorldContextObject);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);
	void PushConfirmScreenToModalStackAsync(EConfirmUIType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmButtonType)> ButtonClickedCallback);

	UFUNCTION(BlueprintCallable)
	void EnqueuePushWidget(FText InText);

private:
	void ShowNextPushWidget();

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;


private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;

	TQueue<FPushWidgetRequest> PushWidgetQueue;

	bool bIsShowing = false;

	FTimerHandle	PushTimerHandle;

	int32 CurrentPushWidgetNum = 0;
};
