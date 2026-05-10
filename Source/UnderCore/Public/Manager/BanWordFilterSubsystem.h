// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BanWordFilterSubsystem.generated.h"

struct FBanWordNode;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UBanWordFilterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ContainsBadWord(const FString& Input);


protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void DeleteNode(FBanWordNode* Node);


private:
	TObjectPtr<UDataTable>		CachedBanWordDataTable;
	TObjectPtr<FBanWordNode>	RootNode;
	
};
