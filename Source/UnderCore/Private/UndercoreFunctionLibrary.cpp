// Fill out your copyright notice in the Description page of Project Settings.


#include "UndercoreFunctionLibrary.h"
#include "UndercoreGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Ability/UndercoreAbilitySystemComponent.h"
#include "Settings/MainMenuDeveloperSettings.h"
#include "Internationalization/StringTableRegistry.h"
#include "Interface/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "ScalableFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Save/UndercoreSaveGame.h"
#include "PlayerState/UndercorePlayerState.h"
#include "Character/UndercoreMainCharacter.h"



UUndercoreAbilitySystemComponent* UUndercoreFunctionLibrary::GetUndercoreASCFromActor(AActor* InActor)
{	
	if (InActor)
	{
		return CastChecked<UUndercoreAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	}

	return nullptr;
}

void UUndercoreFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	if (InActor)
	{
		UUndercoreAbilitySystemComponent* ASC = GetUndercoreASCFromActor(InActor);

		if (ASC)
		{
			if (!ASC->HasMatchingGameplayTag(TagToAdd))
			{
				ASC->AddLooseGameplayTag(TagToAdd);
			}
		}
	}
}

void UUndercoreFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	if (InActor)
	{
		UUndercoreAbilitySystemComponent* ASC = GetUndercoreASCFromActor(InActor);

		if (ASC)
		{
			if (ASC->HasMatchingGameplayTag(TagToRemove))
			{
				ASC->RemoveLooseGameplayTag(TagToRemove);
			}
		}
	}
}

bool UUndercoreFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	if (InActor)
	{
		UUndercoreAbilitySystemComponent* ASC = GetUndercoreASCFromActor(InActor);

		if (ASC)
		{
			return ASC->HasMatchingGameplayTag(TagToCheck);
		}
	}

	return false;
}

void UUndercoreFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EUndercoreConfirmType& OutType)
{
	OutType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EUndercoreConfirmType::Yes : EUndercoreConfirmType::No;
}

TSoftClassPtr<UWidget_ActivatableBase> UUndercoreFunctionLibrary::GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "FrontEnd.Widget"))FGameplayTag InWidgetTag)
{
	const UMainMenuDeveloperSettings* FrontendDeveloperSettings = GetDefault<UMainMenuDeveloperSettings>();

	check(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag));
	
	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

FText UUndercoreFunctionLibrary::GetTextFromStringData(const FString& Key)
{
	return FText::FromStringTable("GameText", Key);
}

FString UUndercoreFunctionLibrary::GetStringFromStringData(const FString& Key)
{
	return FText::FromStringTable("GameText", Key).ToString();
}

UPawnCombatComponent* UUndercoreFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	if (InActor->Implements<UPawnCombatInterface>())
	{
		if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
		{
			return PawnCombatInterface->GetPawnCombatComponent();
		}
	}
	return nullptr;
}

UPawnCombatComponent* UUndercoreFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EUndercoreValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EUndercoreValidType::Valid : EUndercoreValidType::InValid;

	return CombatComponent;
}

bool UUndercoreFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UUndercoreFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UUndercoreFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

	if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
	{
		return UndercoreGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
	{
		return UndercoreGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference < -135.f || OutAngleDifference > 135.f)
	{
		return UndercoreGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
	{
		return UndercoreGameplayTags::Shared_Status_HitReact_Right;
	}

	return UndercoreGameplayTags::Shared_Status_HitReact_Front;
}

UUndercoreSaveGame* UUndercoreFunctionLibrary::SaveCurrentGameData(ACharacter* InCharacter)
{
	if (USaveGame* SaveData = UGameplayStatics::CreateSaveGameObject(UUndercoreSaveGame::StaticClass()))
	{
		if (UUndercoreSaveGame* Data = Cast<UUndercoreSaveGame>(SaveData))
		{
			if (APlayerState* State = UGameplayStatics::GetPlayerState(InCharacter, 0))
			{
				if (AUndercorePlayerState* PlayerState = Cast<AUndercorePlayerState>(State))
				{
					Data->PlayerNickName = PlayerState->GetNickName();
					Data->PlayerLocation = InCharacter->GetActorLocation();
					Data->CurrentDate = FDateTime::UtcNow().ToString(TEXT("%Y/%m/%d"));
					Data->CurrentTime = FDateTime::UtcNow().ToString(TEXT("%H:%M"));
					Data->CurrentQuestSteps = UUndercoreQuestManager::Get(InCharacter)->GetCurrentQuestSteps();
					Data->CurrentQuestStepIndex = UUndercoreQuestManager::Get(InCharacter)->GetCurrentStepIndex();

					UGameplayStatics::SaveGameToSlot(Data, TEXT("Undercore"), 0);

					return Data;
				}
			}
		}
	}

	return nullptr;
}
