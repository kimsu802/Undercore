// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DoorActor.h"

void ADoorActor::Interact(ACharacter* InCharacter)
{
	if (CanInteract())
	{
		OpenDoor(InCharacter);
	}
}

bool ADoorActor::CanInteract()
{
	return true;
}
