// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UndercoreEnemyCharacter.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Combat/EnemyCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/EnemyPoolingManager.h"
#include "Controller/UndercoreAIController.h"
#include "BrainComponent.h"
#include "Manager/Quest/UndercoreQuestManager.h"
#include "UndercoreFunctionLibrary.h"


AUndercoreEnemyCharacter::AUndercoreEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	ArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmorMesh"));
	ArmorMesh->SetupAttachment(GetMesh());
	ArmorMesh->SetLeaderPoseComponent(GetMesh());

	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh"));
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetLeaderPoseComponent(GetMesh());

	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LegMesh->SetupAttachment(GetMesh());
	LegMesh->SetLeaderPoseComponent(GetMesh());

	BracerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BracerMesh"));
	BracerMesh->SetupAttachment(GetMesh());
	BracerMesh->SetLeaderPoseComponent(GetMesh());

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUndercoreEnemyCharacter::OnBodyCollisionBoxBeginOverlap);


	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUndercoreEnemyCharacter::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* AUndercoreEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void AUndercoreEnemyCharacter::ActiveEnemy(const FVector& Location, const FRotator& Rotation)
{
	bIsActive = true;

	SetActorLocation(Location);
	SetActorRotation(Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	InitEnemyStartUpData();

	//if (AAIController* AI = GetController<AAIController>())
	//{
	//	if (UBrainComponent* Brain = AI->GetBrainComponent())
	//	{
	//		AI->RunBehaviorTree(BTAsset);
	//	}
	//}
}

void AUndercoreEnemyCharacter::DeactiveEnemy()
{
	bIsActive = false;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
		AbilitySystemComponent->RemoveAllGameplayCues();
		AbilitySystemComponent->ClearAllAbilities();
	}

	UUndercoreQuestManager::Get(this)->OnEnemyDead(this);

	/*if (AAIController* AI = GetController<AAIController>())
	{
		if (UBrainComponent* Brain = AI->GetBrainComponent())
		{
			Brain->StopLogic(TEXT("Pooled"));
		}
	}*/
}

void AUndercoreEnemyCharacter::SetPoolManager(AEnemyPoolingManager* Manager)
{
	CachedPoolingManager = Manager;
}

void AUndercoreEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AUndercoreEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
}

void AUndercoreEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UUndercoreFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void AUndercoreEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}

			}));

}
