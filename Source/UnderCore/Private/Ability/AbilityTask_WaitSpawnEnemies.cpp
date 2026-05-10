// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityTask_WaitSpawnEnemies.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/UndercoreEnemyCharacter.h"
#include "NavigationSystem.h"
#include "Manager/EnemyPoolingManager.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AUndercoreEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);

	Node->CachedEventTag = EventTag;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;

	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.AddUObject(this, &UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(DelegateHandle);

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventReceived(const FGameplayEventData* InPayload)
{
	TArray<AActor*> Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyPoolingManager::StaticClass(), Results);
	if (!Results.IsEmpty())
	{
		for (const auto& p : Results)
		{
			if (AEnemyPoolingManager* Manager = Cast<AEnemyPoolingManager>(p))
			{
				if (Manager->IsEnemyClassEqual(CachedSoftEnemyClassToSpawn))
				{
					OnEnemyClassLoaded(Manager);
				}
			}
		}
	}

	else if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
		);
	}
	
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AUndercoreEnemyCharacter*>());
		}

		EndTask();
	}
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AUndercoreEnemyCharacter*>());
		}

		EndTask();

		return;
	}

	TArray<AUndercoreEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		FVector RandomLocation;

		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

		RandomLocation += FVector(0.f, 0.f, 150.f);

		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		AUndercoreEnemyCharacter* SpawnedEnemy = World->SpawnActor<AUndercoreEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation, SpawnParam);

		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AUndercoreEnemyCharacter*>());
		}
	}

	EndTask();
}

void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded(AEnemyPoolingManager* InManager)
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	if (!LoadedClass || !World || !InManager)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AUndercoreEnemyCharacter*>());
		}

		EndTask();

		return;
	}

	TArray<AUndercoreEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		FVector RandomLocation;

		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);

		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		RandomLocation += FVector(0.f, 0.f, 150.f);

		AUndercoreEnemyCharacter* SpawnedEnemy = InManager->AcquireEnemy(RandomLocation,SpawnFacingRotation);

		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (!SpawnedEnemies.IsEmpty())
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
		else
		{
			DidNotSpawn.Broadcast(TArray<AUndercoreEnemyCharacter*>());
		}
	}

	EndTask();
}
