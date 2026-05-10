// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToFaceTarget();

private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset);
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool HasReachingAnglePrecision(APawn* QueryPawn, AActor* TargetActor);

private:
	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision;;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector	InTargetToFaceKey;
};
