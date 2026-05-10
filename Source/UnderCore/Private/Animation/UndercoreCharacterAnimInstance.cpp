// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UndercoreCharacterAnimInstance.h"
#include "Character/UndercoreMainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Animation/UndercoreMainAnimInstance.h"


void UUndercoreCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AUndercoreBaseCharacter* Character = Cast<AUndercoreBaseCharacter>(TryGetPawnOwner()))
	{
		OwningCharacter = Character;

		if (OwningCharacter)
		{
			if (UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement())
			{
				OwningCharacterMovement = MovementComponent;
			}
		}
	}
}

void UUndercoreCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwningCharacterMovement)
	{
		bHasAcceleration = OwningCharacterMovement->GetCurrentAcceleration().SizeSquared2D() > 0.f ? true : false;
	}

	if (OwningCharacter)
	{
		GroundSpeed = OwningCharacter->GetVelocity().Size2D();
		LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
	}

}
