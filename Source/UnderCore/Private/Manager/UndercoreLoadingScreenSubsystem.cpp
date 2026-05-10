// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UndercoreLoadingScreenSubsystem.h"
#include "Settings/UndercoreLoadingScreenSettings.h"
#include "PreLoadScreenManager.h"
#include "Blueprint/UserWidget.h"
#include "Interface/UndercoreLoadingScreenInterface.h"


UWorld* UUndercoreLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UUndercoreLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

ETickableTickType UUndercoreLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool UUndercoreLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UUndercoreLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UUndercoreLoadingScreenSubsystem, STATGROUP_Tickables);
}

bool UUndercoreLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UUndercoreLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);

}

void UUndercoreLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UUndercoreLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();

}

void UUndercoreLoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UUndercoreLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	if (IsPreLoadScreenActive())
	{
		return;
	}

	if (ShouldShowLoadingScreen())
	{
		TryDisplayLoadingScreenIfNone();

		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.f;

		SetTickableTickType(ETickableTickType::Never);
	}
}

bool UUndercoreLoadingScreenSubsystem::IsPreLoadScreenActive()
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		PreLoadScreenManager->HasValidActivePreLoadScreen();
	}

	return false;
}

bool UUndercoreLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UUndercoreLoadingScreenSettings* LoadingScreenSettings = GetDefault<UUndercoreLoadingScreenSettings>();

	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}

	return false;
}

bool UUndercoreLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");

		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");

		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't begun play yet");

		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player Controller is not valid yet");

		return true;
	}



	return false;
}

void UUndercoreLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	const UUndercoreLoadingScreenSettings* LoadingScreenSettings = GetDefault<UUndercoreLoadingScreenSettings>();

	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);

	check(CreatedWidget);

	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreenWidget.ToSharedRef(),
		1000
	);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UUndercoreLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());

	CachedCreatedLoadingScreenWidget.Reset();

	NotifyLoadingScreenVisibilityChanged(false);
}

void UUndercoreLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
		{
			continue;
		}

		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			if (PC->Implements<UUndercoreLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IUndercoreLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IUndercoreLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UUndercoreLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IUndercoreLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IUndercoreLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}

				}
			}
		}
	}
}

