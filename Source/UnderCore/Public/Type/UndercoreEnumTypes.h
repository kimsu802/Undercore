// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EUndercoreSuccessType : uint8
{
	Successful,
	Failed
};

UENUM(BlueprintType)
enum class EConfirmUIType : uint8
{
	OK,
	YesNo,
	OKCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

UENUM()
enum class EUndercoreConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EUndercoreValidType : uint8
{
	Valid,
	InValid
};

UENUM(BlueprintType)
enum class EQuestConditionType : uint8
{
	None,
	Kill,
	Location,
	Dialogue,
	Interact
};