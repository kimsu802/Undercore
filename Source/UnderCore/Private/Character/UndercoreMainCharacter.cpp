// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UndercoreMainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/UndercoreInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "UndercoreGameplayTags.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Ability/UndercoreGameplayAbility.h"
#include "UndercoreFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Interface/InteractableInterface.h"
#include "UI/Interaction/InteractionWidget.h"
#include "Controller/UndercoreMainController.h"
#include "Item/InteractableActor.h"
#include "CharacterTrajectoryComponent.h"
#include "Manager/MainMenuUISubsystem.h"
#include "DataAsset/DataAsset_MainStartUpData.h"
#include "Component/Combat/MainCombatComponent.h"
#include "Manager/Dialogue/UndercoreDialogueManager.h"

AUndercoreMainCharacter::AUndercoreMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(41.f, 97.f);

	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);

	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectory"));
	MainCombatComponent = CreateDefaultSubobject<UMainCombatComponent>(TEXT("MainCombatComponent"));
}

UPawnCombatComponent* AUndercoreMainCharacter::GetPawnCombatComponent() const
{
	return MainCombatComponent;
}

void AUndercoreMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionSphere)
	{
		InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionOverlapBegin);
		InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionOverlapEnd);
	}

}

void AUndercoreMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InteractionSphere)
	{
		InteractionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnInteractionOverlapBegin);
		InteractionSphere->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnInteractionOverlapEnd);
	}

	Super::EndPlay(EndPlayReason);
}

void AUndercoreMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		if (Subsystem)
		{
			if (InputDataAsset)
			{
				Subsystem->AddMappingContext(InputDataAsset->GetDefaultMappingContext(), 0);

				if (UUndercoreInputComponent* UndercoreInputComponent = Cast<UUndercoreInputComponent>(PlayerInputComponent))
				{
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AUndercoreMainCharacter::Input_Look);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AUndercoreMainCharacter::Input_Move);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &AUndercoreMainCharacter::Input_Jump);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Walk, ETriggerEvent::Started, this, &AUndercoreMainCharacter::Input_Walk);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_FastRun, ETriggerEvent::Started, this, &AUndercoreMainCharacter::Input_FastRun_Started);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_FastRun, ETriggerEvent::Completed, this, &AUndercoreMainCharacter::Input_FastRun_Completed);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Interact, ETriggerEvent::Started, this, &AUndercoreMainCharacter::Interact);
					UndercoreInputComponent->BindNativeInputAction(InputDataAsset, UndercoreGameplayTags::InputTag_Escape, ETriggerEvent::Started, this, &AUndercoreMainCharacter::Input_Escape);


					UndercoreInputComponent->BindAbilityInputAction(InputDataAsset, this, &AUndercoreMainCharacter::Input_AbilityInputPressed, &AUndercoreMainCharacter::Input_AbilityInputReleased);
				}
			}
		}
	}

}

void AUndercoreMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
		}
	}

	if (!InteractionWidgetClass.IsNull())
	{
		UClass* LoadedClass = InteractionWidgetClass.LoadSynchronous();

		check(LoadedClass);

		InteractionWidget = Cast<UInteractionWidget>(CreateWidget(Cast<AUndercoreMainController>(NewController), LoadedClass));

		check(InteractionWidget);

		InteractionWidget->AddToViewport(0);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AUndercoreMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateInteractionTarget();
}

void AUndercoreMainCharacter::OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInteractableActor* InterableActor = Cast<AInteractableActor>(OtherActor))
	{
		OverlappedActors.Add(InterableActor);
	}
}

void AUndercoreMainCharacter::OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractableActor* InterableActor = Cast<AInteractableActor>(OtherActor))
	{
		if (OverlappedActors.Contains(InterableActor))
		{
			OverlappedActors.Remove(InterableActor);
		}
	}
}

void AUndercoreMainCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D& MoveVector = InputActionValue.Get<FVector2D>();
	const FRotator& ControlRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	if (MoveVector.X != 0.f)
	{
		const FVector& RightDirection = ControlRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MoveVector.X);
	}

	if (MoveVector.Y != 0.f)
	{
		const FVector& ForwardDirection = ControlRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MoveVector.Y);
	}
	
	
}

void AUndercoreMainCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D& MoveVector = InputActionValue.Get<FVector2D>();

	if (MoveVector.X != 0.f)
	{
		AddControllerYawInput(MoveVector.X);
	}

	if (MoveVector.Y != 0.f)
	{
		AddControllerPitchInput(MoveVector.Y);
	}
}

void AUndercoreMainCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		Jump();
	}
}

void AUndercoreMainCharacter::Input_Walk(const FInputActionValue& InputActionValue)
{
	if (UUndercoreFunctionLibrary::NativeDoesActorHaveTag(this, UndercoreGameplayTags::Shared_Status_Walking))
	{
		UUndercoreFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, UndercoreGameplayTags::Shared_Status_Walking);
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	else
	{
		UUndercoreFunctionLibrary::AddGameplayTagToActorIfNone(this, UndercoreGameplayTags::Shared_Status_Walking);
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}
}

void AUndercoreMainCharacter::Input_FastRun_Started(const FInputActionValue& InputActionValue)
{
	UUndercoreFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, UndercoreGameplayTags::Shared_Status_Walking);
	UUndercoreFunctionLibrary::AddGameplayTagToActorIfNone(this, UndercoreGameplayTags::Player_Status_FastRunning);
	GetCharacterMovement()->MaxWalkSpeed = 650.f;
}

void AUndercoreMainCharacter::Input_FastRun_Completed(const FInputActionValue& InputActionValue)
{
	UUndercoreFunctionLibrary::RemoveGameplayTagFromActorIfFound(this, UndercoreGameplayTags::Player_Status_FastRunning);
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void AUndercoreMainCharacter::Input_Escape()
{
	if (UMainMenuUISubsystem* System = UMainMenuUISubsystem::Get(this))
	{
		System->PushSoftWidgetToStackAsync(
			UndercoreGameplayTags::FrontEnd_WidgetStack_GameMenu,
			UUndercoreFunctionLibrary::GetFrontendSoftWidgetClassByTag(UndercoreGameplayTags::FrontEnd_Widget_MainMenuScreen),
			[](EAsyncPushWidgetState InState, UWidget_ActivatableBase* CreatedWidget)
			{
				return;
			}
		);
	}
}

void AUndercoreMainCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AUndercoreMainCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AUndercoreMainCharacter::UpdateInteractionTarget()
{
	AInteractableActor*	BestActor = nullptr;
	float BestScore = -9999.f;

	FVector PlayerLoc = GetActorLocation();
	FVector Forward = GetControlRotation().Vector();

	for (const auto& Actor : OverlappedActors)
	{
		if (!Actor.IsValid()) continue;

		FVector ToTarget = Actor->GetActorLocation() - PlayerLoc;
		float Distance = ToTarget.Size();

		float DistanceScore = 1.f - (Distance / 500.f);

		float Dot = FVector::DotProduct(Forward, ToTarget.GetSafeNormal());
		float DirectionScore = (Dot + 1.f) * 0.5f;

		float Score = DistanceScore * 0.4f + DirectionScore * 0.6f;

		if (Score > BestScore)
		{
			BestScore = Score;
			BestActor = Actor.Get();
		}
	}

	SetCurrentInteractionTarget(BestActor);
}

void AUndercoreMainCharacter::SetCurrentInteractionTarget(AInteractableActor* NewTarget)
{
	if (CurrentInteractionTarget.IsValid() && CurrentInteractionTarget == NewTarget)
	{
		FVector WorldPos = CurrentInteractionTarget->GetActorLocation() + FVector(0, 0, 100.f);

		FVector2D ScreenPos;
		GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(WorldPos, ScreenPos);

		InteractionWidget->SetPositionInViewport(ScreenPos);

		return;
	}

	if (CurrentInteractionTarget.IsValid())
	{
		ToggleOutline(CurrentInteractionTarget.Get(), false);

		if (InteractionWidget)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	CurrentInteractionTarget = NewTarget;

	if (CurrentInteractionTarget.IsValid())
	{
		ToggleOutline(CurrentInteractionTarget.Get(), true);

		if (InteractionWidget)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			InteractionWidget->UpdateInteractionWidgetInfo(CurrentInteractionTarget.Get()->GetBindingKeyNameByIndex(0));
			FVector WorldPos = CurrentInteractionTarget->GetActorLocation() + FVector(0, 0, 100.f);

			FVector2D ScreenPos;
			GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(WorldPos, ScreenPos);

			InteractionWidget->SetPositionInViewport(ScreenPos);
		}
	}
}

void AUndercoreMainCharacter::ToggleOutline(AInteractableActor* Actor, bool bEnable)
{
	TArray<UPrimitiveComponent*> Components;
	Actor->GetComponents(Components);

	for (auto Comp : Components)
	{
		Comp->SetRenderCustomDepth(bEnable);
	}
}

void AUndercoreMainCharacter::Interact()
{
	if (CurrentInteractionTarget.IsValid() && OverlappedActors.Contains(CurrentInteractionTarget) && !UUndercoreDialogueManager::Get(this)->GetbIsDialogueActive())
	{
		CurrentInteractionTarget->Interact(this);
	}
}
