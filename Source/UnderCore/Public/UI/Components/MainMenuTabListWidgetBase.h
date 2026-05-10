// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "MainMenuTabListWidgetBase.generated.h"

class UMainMenuCommonButtonBase;
/**
 * 
 */
UCLASS(BlueprintType, Abstract, meta = (DisableNativeTick))
class UNDERCORE_API UMainMenuTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);


private:
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fronted Tab List Settings", meta = (AllowPrivateAccess = true, ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreiewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fronted Tab List Settings", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainMenuCommonButtonBase> TabButtonEntryWidgetClass;


};
