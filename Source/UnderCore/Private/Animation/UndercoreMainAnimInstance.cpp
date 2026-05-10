// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UndercoreMainAnimInstance.h"
#include "Character/UndercoreMainCharacter.h"


void UUndercoreMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningMainCharacter = Cast<AUndercoreMainCharacter>(OwningCharacter);
	}
}

void UUndercoreMainAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}

	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}

