// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UndercoreBaseCharacter.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Ability/UndercoreAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AUndercoreBaseCharacter::AUndercoreBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UUndercoreAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UUndercoreAttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void AUndercoreBaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AUndercoreBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AUndercoreBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPawnCombatComponent* AUndercoreBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

