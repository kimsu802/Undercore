// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_KeyRemapScreen.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo) : CachedInputTypeListenTo(InInputTypeToListenTo)
	{}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnInputPreProcessorKeyPressedDelegate	OnInputPreProcessorKeyPressed;
	
	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectedCanceledDelegate, const FString& /*CanceledReason*/)
	FOnInputPreProcessorKeySelectedCanceledDelegate OnInputPreProcessorKeySelectedCanceled;

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{

	}
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("키 입력이 취소되었습니다."));

			return;
		}

		switch (CachedInputTypeListenTo)
		{
			case ECommonInputType::MouseAndKeyboard:
			{
				if (InPressedKey.IsGamepadKey())
				{
					OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("게임 패드 입력이 감지 되었습니다."));

					return;
				}
				break;
			}
			case ECommonInputType::Gamepad:
			{
				if (!InPressedKey.IsGamepadKey())
				{
					OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("게임 패드가 아닌 다른 입력이 감지 되었습니다."));

					return;
				}
				break;
			}
			default:
				break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}



private:
	ECommonInputType CachedInputTypeListenTo;
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreProcessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType);

	CachedInputPreProcessor->OnInputPreProcessorKeyPressed.BindUObject(this, &UWidget_KeyRemapScreen::OnValidKeyPressedDetected);
	CachedInputPreProcessor->OnInputPreProcessorKeySelectedCanceled.BindUObject(this, &UWidget_KeyRemapScreen::OnKeySelectCanceled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreProcessor,-1);

	FString InputDeviceName;

	switch (CachedDesiredInputType)
	{
		case ECommonInputType::MouseAndKeyboard:
		{
			InputDeviceName = TEXT("마우스 & 키보드");
		}
			break;
		case ECommonInputType::Gamepad:
		{
			InputDeviceName = TEXT("게임패드");
		}
			break;
		default:
			break;
	}
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();


	if (CachedInputPreProcessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreProcessor);

		CachedInputPreProcessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{

	RequestDeactivateWidget(
		[this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([PreDeactivateCallback, this](float DeltaTime)->bool
			{
				PreDeactivateCallback();

				DeactivateWidget();

				return false;
			})
	);
}
