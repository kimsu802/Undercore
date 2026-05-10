// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/UndercoreBaseAnimInstance.h"
#include "UndercoreCharacterAnimInstance.generated.h"

class AUndercoreBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreCharacterAnimInstance : public UUndercoreBaseAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AUndercoreBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;

};
