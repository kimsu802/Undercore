// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/PawnCombatInterface.h"
#include "UndercoreBaseCharacter.generated.h"


class UMotionWarpingComponent;
class UInputMappingContext;
class UUndercoreAbilitySystemComponent;
class UUndercoreAttributeSet;
class UDataAsset_StartUpDataBase;
class UPawnCombatComponent;

UCLASS()
class UNDERCORE_API AUndercoreBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUndercoreBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystemComponent")
	TObjectPtr<UUndercoreAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttributeSet")
	TObjectPtr<UUndercoreAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character StartUp Data")
	TSoftObjectPtr<UDataAsset_StartUpDataBase>	CharacterStartUpData;
};
