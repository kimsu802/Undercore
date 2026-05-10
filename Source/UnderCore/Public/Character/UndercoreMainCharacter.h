// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UndercoreBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "UndercoreMainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class USphereComponent;
class UInteractionWidget;
class AInteractableActor;
class UCharacterTrajectoryComponent;
class UDataAsset_MainStartUpData;
class UMainCombatComponent;

/**
 * 
 */
UCLASS()
class UNDERCORE_API AUndercoreMainCharacter : public AUndercoreBaseCharacter
{
	GENERATED_BODY()
	
public:
	AUndercoreMainCharacter();

	FORCEINLINE UMainCombatComponent* GetMainCombatComponent() const { return MainCombatComponent; }

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime);

private:
	UFUNCTION()
	void OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Input
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Walk(const FInputActionValue& InputActionValue);
	void Input_FastRun_Started(const FInputActionValue& InputActionValue);
	void Input_FastRun_Completed(const FInputActionValue& InputActionValue);
	void Input_Escape();

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

#pragma region Interact
	void UpdateInteractionTarget();
	void SetCurrentInteractionTarget(AInteractableActor* NewTarget);
	void ToggleOutline(AInteractableActor* Actor, bool bEnable);
	void Interact();
#pragma endregion

private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent>				CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent>				CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent>				InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterTrajectoryComponent>	CharacterTrajectory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMainCombatComponent>			MainCombatComponent;

#pragma endregion

#pragma region DataAssets
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = true))
	TObjectPtr<UDataAsset_InputConfig>			InputDataAsset;
#pragma endregion

#pragma region Interaction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = true))
	TSoftClassPtr<UInteractionWidget>			InteractionWidgetClass;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AInteractableActor>>	OverlappedActors;

	UPROPERTY(Transient)
	TWeakObjectPtr<AInteractableActor>			CurrentInteractionTarget;
	
	UPROPERTY(Transient)
	TObjectPtr<UInteractionWidget>				InteractionWidget;
#pragma endregion

};
