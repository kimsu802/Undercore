// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class AUndercoreMainCharacter;
struct FDialogueData;

UCLASS(Abstract,BlueprintType, meta = (DisableNativeTick))
class UNDERCORE_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual void	Interact(ACharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void			BP_Interact(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable)
	FName			GetBindingKeyNameByIndex(int index);

private:
	virtual bool	CanInteract();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key Binding Name", meta = (AllowPrivateAccess = true))
	TArray<FName>	BindingKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Table ID", meta = (AllowPrivateAccess = true))
	FName			TableID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Table ID", meta = (AllowPrivateAccess = true))
	bool			InCombatFight = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Table ID", meta = (AllowPrivateAccess = true))
	bool			IsInEquippedFight = false;

	TWeakObjectPtr<AUndercoreMainCharacter>	CachedMainCharacter;
};
