// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UndercoreBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHaveTag(struct FGameplayTag InTag) const;
};
