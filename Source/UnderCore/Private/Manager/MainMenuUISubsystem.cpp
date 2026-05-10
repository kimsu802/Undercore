// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MainMenuUISubsystem.h"
#include "Engine/AssetManager.h"
#include "UndercoreDebugHelper.h"
#include "UI/Widget_ActivatableBase.h" 
#include "UI/Widget_PrimaryLayout.h"
#include "UI/Widget_ActivatablePush.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widget_ConfirmUI.h"
#include "UndercoreFunctionLibrary.h"
#include "UndercoreGameplayTags.h"
#include "Settings/MainMenuDeveloperSettings.h"

UMainMenuUISubsystem* UMainMenuUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UMainMenuUISubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UMainMenuUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UMainMenuUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
    check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;
}

void UMainMenuUISubsystem::PushSoftWidgetToStackAsync( const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
    if (InSoftWidgetClass.IsNull())
        return;

    UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        InSoftWidgetClass.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallback]()
            {
                UClass* LoadedWidgetClass = InSoftWidgetClass.Get();

                if (!LoadedWidgetClass || !CreatedPrimaryLayout)
                    return;

                UCommonActivatableWidgetContainerBase* FoundWidgetStack =
                    CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

                 check(FoundWidgetStack)

                    UWidget_ActivatableBase* CreatedWidget =
                        FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
                            LoadedWidgetClass,
                            [AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
                            {
                                AsyncPushStateCallback(
                                    EAsyncPushWidgetState::OnCreatedBeforePush,
                                    &CreatedWidgetInstance
                                );
                            });

                    AsyncPushStateCallback(
                        EAsyncPushWidgetState::AfterPush,
                        CreatedWidget
                    );
                
            }));
}

void UMainMenuUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmUIType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmButtonType)> ButtonClickedCallback)
{
    UConfirmScreenInfoObject* CreateInfoObject = nullptr;

    switch (InScreenType)
    {
    case EConfirmUIType::OK:
        CreateInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMsg);
        break;
    case EConfirmUIType::YesNo:
        CreateInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);
        break;
    case EConfirmUIType::OKCancel:
        CreateInfoObject = UConfirmScreenInfoObject::CreateOKCancelScreen(InScreenTitle, InScreenMsg);
        break;
    }

    PushSoftWidgetToStackAsync(
        UndercoreGameplayTags::FrontEnd_WidgetStack_Modal,
        UUndercoreFunctionLibrary::GetFrontendSoftWidgetClassByTag(UndercoreGameplayTags::FrontEnd_Widget_ConfirmScreen),
        [ButtonClickedCallback, CreateInfoObject](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
        {
            if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
            {
                UWidget_ConfirmUI* CreatedConfirmUI = CastChecked<UWidget_ConfirmUI>(PushedWidget);
                CreatedConfirmUI->InitConfirmUI(CreateInfoObject, ButtonClickedCallback);
            }
        }
    );

}

void UMainMenuUISubsystem::EnqueuePushWidget(FText InText)
{
    if (InText.IsEmpty())
        return;

    const UMainMenuDeveloperSettings* Settings = GetDefault<UMainMenuDeveloperSettings>();

    if (!Settings)
    {
        return;
    }

    if (CurrentPushWidgetNum == Settings->MaximumPushWidgetNum)
        return;

    TSoftClassPtr<UWidget_ActivatableBase> PushWidgetSoftClass = UUndercoreFunctionLibrary::GetFrontendSoftWidgetClassByTag(UndercoreGameplayTags::Push_Widget_Default);

    FPushWidgetRequest Request;
    Request.Message = InText;
    Request.SoftWidgetClass = PushWidgetSoftClass;

    PushWidgetQueue.Enqueue(Request);
    CurrentPushWidgetNum++;

    if (!bIsShowing)
    {
        ShowNextPushWidget();
    }
}

void UMainMenuUISubsystem::ShowNextPushWidget()
{
    if (PushWidgetQueue.IsEmpty())
    {
        bIsShowing = false;
        return;
    }

    bIsShowing = true;

    FPushWidgetRequest Request;
    PushWidgetQueue.Dequeue(Request);
    CurrentPushWidgetNum--;

    PushSoftWidgetToStackAsync(UndercoreGameplayTags::FrontEnd_WidgetStack_FrontEnd, Request.SoftWidgetClass,
        [this, Request](EAsyncPushWidgetState State, UWidget_ActivatableBase* CreatedWidget)
        {
            if (State == EAsyncPushWidgetState::OnCreatedBeforePush)
            {
                UWidget_ActivatablePush* PushWidget = Cast<UWidget_ActivatablePush>(CreatedWidget);

                check(PushWidget);

                //푸시 위젯 텍스트 변경
                PushWidget->SetPushText(Request.Message);
            }

            if (State == EAsyncPushWidgetState::AfterPush)
            {
                // 푸시 이후에는 타이머 설정
                GetWorld()->GetTimerManager().SetTimer(
                    PushTimerHandle,
                    [this, CreatedWidget]()
                    {
                        CreatedWidget->DeactivateWidget();
                        ShowNextPushWidget();
                    },
                    2.0f,
                    false
                );
            }
        }
    );
}
