// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UndercoreBaseAnimInstance.h"
#include "GameplayTagContainer.h"
#include "UndercoreFunctionLibrary.h"

bool UUndercoreBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag InTag) const
{
	if (APawn* Pawn = TryGetPawnOwner())
	{
		return UUndercoreFunctionLibrary::NativeDoesActorHaveTag(Pawn, InTag);
	}

	return false;
}
