// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/MainMenuTabListWidgetBase.h"
#include "UI/Components/MainMenuCommonButtonBase.h"
#include "Editor/WidgetCompilerLog.h"


void UMainMenuTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UMainMenuCommonButtonBase* FoundButton = Cast<UMainMenuCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR
void UMainMenuTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified.") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif