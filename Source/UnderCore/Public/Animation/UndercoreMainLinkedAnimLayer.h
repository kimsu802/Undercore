// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/UndercoreBaseAnimInstance.h"
#include "UndercoreMainLinkedAnimLayer.generated.h"

class UUndercoreMainAnimInstance;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreMainLinkedAnimLayer : public UUndercoreBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UUndercoreMainAnimInstance* GetMainAnimInstance() const;
	
};
