// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AsyncAction_PushSoftWidget.h"
#include "Manager/MainMenuUISubsystem.h"
#include "UI/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, UPARAM(meta = (Categories = "FrontEnd.WidgetStack"))FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
    if (GEngine)
    {
        if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            UAsyncAction_PushSoftWidget* Node =  NewObject<UAsyncAction_PushSoftWidget>();
            Node->CachedOwningWorld = World;
            Node->CachedOwningPC = OwningPlayerController;
            Node->CachedSoftWidgetClass = InSoftWidgetClass;
            Node->CachedWidgetStackTag = InWidgetStackTag;
            Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

            Node->RegisterWithGameInstance(World);
            
            return Node;
        }
    }

    return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
    if (UMainMenuUISubsystem* System = UMainMenuUISubsystem::Get(CachedOwningWorld.Get()))
    {
        System->PushSoftWidgetToStackAsync(
            CachedWidgetStackTag,
            CachedSoftWidgetClass,
            [this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
            {
                switch (InPushState)
                {
                    case EAsyncPushWidgetState::OnCreatedBeforePush:
                    {
                        PushedWidget->SetOwningPlayer(CachedOwningPC.Get());

                        OnWidgetCreatedBeforePush.Broadcast(PushedWidget);

                        break;
                    }
                    case EAsyncPushWidgetState::AfterPush: 
                    {
                        AfterPush.Broadcast(PushedWidget);
                        if (bCachedFocusOnNewlyPushedWidget)
                        { 
                            if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
                            {
                                WidgetToFocus->SetFocus();
                            }
                        }
                        SetReadyToDestroy();
                        break;
                    }
                    default:
                        break;
                }
            });
    }
}
