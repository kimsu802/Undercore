// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UndercoreBaseCharacter.h"
#include "UndercoreEnemyCharacter.generated.h"


class UEnemyCombatComponent;
class USkeletalMeshComponent;
class UDataAsset_EnemyStartUpData;
class AEnemyPoolingManager;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeadDelegate, AActor*, Enemy);

/**
 * 
 */
UCLASS()
class UNDERCORE_API AUndercoreEnemyCharacter : public AUndercoreBaseCharacter
{
	GENERATED_BODY()
public:
	AUndercoreEnemyCharacter();

	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
	FORCEINLINE const FText& GetDisplayName() const { return DisplayName;  }

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	FORCEINLINE bool GetIsActive() const { return bIsActive; }
	void SetIsActive(bool bFlag) { bIsActive = bFlag; }

	UFUNCTION(BlueprintCallable)
	void ActiveEnemy(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable)
	void DeactiveEnemy();

	void SetPoolManager(AEnemyPoolingManager* Manager);

protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void InitEnemyStartUpData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UEnemyCombatComponent> EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent>	ArmorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent>	HelmetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent>	LegMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent>	BracerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent>	LeftHandCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UBoxComponent>	RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName	LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName	RightHandCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText	DisplayName;



private:
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<AEnemyPoolingManager> CachedPoolingManager;
};
