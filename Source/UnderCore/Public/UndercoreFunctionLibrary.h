// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Type/UndercoreEnumTypes.h"
#include "UndercoreFunctionLibrary.generated.h"

struct FGameplayTag;
class UUndercoreAbilitySystemComponent;
class UWidget_ActivatableBase;
class UPawnCombatComponent;
struct FScalableFloat;
class UUndercoreSaveGame;

/**
 * 
 */
UCLASS()
class UNDERCORE_API UUndercoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Undercore|FunctionLibrary")
	static UUndercoreAbilitySystemComponent* GetUndercoreASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary")
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary", meta = (ExpandEnumAsExecs = "OutType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EUndercoreConfirmType& OutType);
	
	UFUNCTION(BlueprintPure, Category = "FrontEnd Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "FrontEnd.Widget"))FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintCallable, Category = "Localization Function Library")
	static FText GetTextFromStringData(const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "Localization Function Library")
	static FString GetStringFromStringData(const FString& Key);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EUndercoreValidType& OutValidType);

	UFUNCTION(BlueprintCallable, Category = "Undercore|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Undercore|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "Undercore|FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintCallable)
	static UUndercoreSaveGame* SaveCurrentGameData(ACharacter* InCharacter);
};
