// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UndercoreMainLinkedAnimLayer.h"
#include "Animation/UndercoreMainAnimInstance.h"

UUndercoreMainAnimInstance* UUndercoreMainLinkedAnimLayer::GetMainAnimInstance() const
{
	return Cast<UUndercoreMainAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
