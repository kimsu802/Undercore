// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/EnemyPoolingManager.h"
#include "EngineUtils.h"
#include "Character/UndercoreEnemyCharacter.h"
#include "UndercoreDebugHelper.h"

// Sets default values
AEnemyPoolingManager::AEnemyPoolingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

AUndercoreEnemyCharacter* AEnemyPoolingManager::AcquireEnemy(const FVector& Location, const FRotator& Rotation)
{
    for (AUndercoreEnemyCharacter* Enemy : EnemyPool)
    {
        if (!Enemy->GetIsActive())
        {
            Enemy->ActiveEnemy(Location,Rotation);
            return Enemy;
        }
    }

    AUndercoreEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AUndercoreEnemyCharacter>(EnemyClass);
    NewEnemy->ActiveEnemy(Location,Rotation);
    EnemyPool.Add(NewEnemy);

    return NewEnemy;
}

void AEnemyPoolingManager::ReleaseEnemy(AUndercoreEnemyCharacter* Enemy)
{
    check(Enemy);

    Enemy->DeactiveEnemy();
}

void AEnemyPoolingManager::RegisterEnemy(AUndercoreEnemyCharacter* Enemy)
{
    if (EnemyPool.Contains(Enemy))
        return;

    Enemy->SetIsActive(true);
    EnemyPool.Add(Enemy);
    Debug::Print(FString::Printf(TEXT("Enemy Registered %d !!"), EnemyPool.Num()));
}

bool AEnemyPoolingManager::IsEnemyClassEqual(TSoftClassPtr<AUndercoreEnemyCharacter> InClass)
{
    return TSoftClassPtr<AUndercoreEnemyCharacter>(EnemyClass) == InClass;
}

// Called when the game starts or when spawned
void AEnemyPoolingManager::BeginPlay()
{
	Super::BeginPlay();

    for (int32 i = 0; i < PoolSize; ++i)
    {
        AUndercoreEnemyCharacter* Enemy = GetWorld()->SpawnActor<AUndercoreEnemyCharacter>(EnemyClass);

        Enemy->DeactiveEnemy();
        Enemy->SetPoolManager(this);

        EnemyPool.Add(Enemy);
    }

    for (TActorIterator<AUndercoreEnemyCharacter> It(GetWorld()); It; ++It)
    {
        AUndercoreEnemyCharacter* Enemy = *It;
        Enemy->SetPoolManager(this);
        RegisterEnemy(Enemy);
    }
}

