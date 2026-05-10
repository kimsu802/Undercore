// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/InteractableActor.h"
#include "DoorActor.generated.h"



/**
 * 
 */
UCLASS()
class UNDERCORE_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	virtual void	Interact(ACharacter* InCharacter) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void	OpenDoor(ACharacter* InCharacter);

private:
	virtual bool	CanInteract() override;
};
