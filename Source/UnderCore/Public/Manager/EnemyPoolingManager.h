// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPoolingManager.generated.h"

class AUndercoreEnemyCharacter;

UCLASS()
class UNDERCORE_API AEnemyPoolingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPoolingManager();

	UFUNCTION(BlueprintCallable, Category = "EnemyPool")
	AUndercoreEnemyCharacter*	AcquireEnemy(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "EnemyPool")
	void						ReleaseEnemy(AUndercoreEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable, Category = "EnemyPool")
	void						RegisterEnemy(AUndercoreEnemyCharacter* Enemy);

	bool IsEnemyClassEqual(TSoftClassPtr<AUndercoreEnemyCharacter> InClass);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void	CollectPlacedEnemies();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<AUndercoreEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	int PoolSize = 10;

private:
	TArray<AUndercoreEnemyCharacter*>	EnemyPool;
};
