// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UndercoreLoadingScreenSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	//UUndercoreLoadingScreenSubsystem* Get(UObject* WorldContextObject) const;
	virtual UWorld*				GetTickableGameObjectWorld() const override;
	virtual void				Tick(float DeltaTime) override;
	virtual ETickableTickType	GetTickableTickType() const override;
	virtual bool				IsTickable() const override;
	virtual TStatId				GetStatId() const override;

protected:
	virtual bool	ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void	Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void	Deinitialize() override;



private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);

	void OnMapPostLoaded(UWorld* LoadedWorld);

	void TryUpdateLoadingScreen();

	bool IsPreLoadScreenActive();

	bool ShouldShowLoadingScreen();

	bool CheckTheNeedToShowLoadingScreen();

	void TryDisplayLoadingScreenIfNone();

	void TryRemoveLoadingScreen();

	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

public:
	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate	OnLoadingReasonUpdated;

private:
	bool bIsCurrentlyLoadingMap = false;

	float HoldLoadingScreenStartUpTime = -1.f;

	FString CurrentLoadingReason;

	TSharedPtr<SWidget>	CachedCreatedLoadingScreenWidget;
};
