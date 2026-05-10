// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/UndercoreQuestTrigger.h"
#include "Components/SphereComponent.h"
#include "Manager/Quest/UndercoreQuestManager.h"
#include "Character/UndercoreMainCharacter.h"

// Sets default values
AUndercoreQuestTrigger::AUndercoreQuestTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	SphereComponent->SetSphereRadius(150.f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	
}

// Called when the game starts or when spawned
void AUndercoreQuestTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(
			this,
			&AUndercoreQuestTrigger::OnOverlapBegin
		);
	}
}

void AUndercoreQuestTrigger::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AUndercoreQuestTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUndercoreMainCharacter* MainCharacter = Cast<AUndercoreMainCharacter>(OtherActor))
	{
		if (UUndercoreQuestManager* Manager = UUndercoreQuestManager::Get(this))
		{
			Manager->OnPlayerEnterTrigger(TriggerID);
		}

		Destroy();
	}
}

void AUndercoreQuestTrigger::SetTriggerRadius(float InRadius)
{
	if (SphereComponent)
	{
		SphereComponent->SetSphereRadius(InRadius);
	}
}

