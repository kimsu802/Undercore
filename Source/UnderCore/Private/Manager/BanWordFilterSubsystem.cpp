// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/BanWordFilterSubsystem.h"
#include "DataTable/UndercoreBanWordDataTable.h"
#include "UndercoreDebugHelper.h"
#include "Settings/MainMenuDeveloperSettings.h"
#include "Type/UndercoreStructTypes.h"

bool UBanWordFilterSubsystem::ContainsBadWord(const FString& Input)
{
	if (!RootNode) return false;

	FBanWordNode* Current = RootNode;

	for (TCHAR Char : Input)
	{
		while (Current != RootNode && !Current->Children.Contains(Char))
		{
			Current = Current->Failure;
		}

		if (Current->Children.Contains(Char))
		{
			Current = Current->Children[Char];
		}

		if (Current->bIsEnd)
		{
			return true; // 금칙어 발견
		}
	}

	return false;
}

void UBanWordFilterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CachedBanWordDataTable)
	{
		if (const UMainMenuDeveloperSettings* MainMenuDeveloperSettings = GetDefault<UMainMenuDeveloperSettings>())
		{
			CachedBanWordDataTable = MainMenuDeveloperSettings->BannedWordDataTable.LoadSynchronous();

			if (CachedBanWordDataTable)
			{
				Debug::Print(TEXT("DT_BanWord was successfully loaded !!"));

				RootNode = new FBanWordNode();

				if (!CachedBanWordDataTable) return;

				TArray<FName> RowNames = CachedBanWordDataTable->GetRowNames();

				// 1. Trie 구성
				for (const FName& Name : RowNames)
				{
					FUndercoreBanWordData* Row = CachedBanWordDataTable->FindRow<FUndercoreBanWordData>(Name, "");
					if (!Row) continue;

					const FString& Word = Row->BanWord;

					FBanWordNode* Current = RootNode;

					for (TCHAR Char : Word)
					{
						if (!Current->Children.Contains(Char))
						{
							Current->Children.Add(Char, new FBanWordNode());
						}

						Current = Current->Children[Char];
					}

					Current->bIsEnd = true;
				}

				// 2. Failure 링크 구성 (BFS)
				TQueue<FBanWordNode*> Queue;

				RootNode->Failure = RootNode;

				for (auto& Pair : RootNode->Children)
				{
					Pair.Value->Failure = RootNode;
					Queue.Enqueue(Pair.Value);
				}

				while (!Queue.IsEmpty())
				{
					FBanWordNode* Current;
					Queue.Dequeue(Current);

					for (auto& Pair : Current->Children)
					{
						TCHAR Char = Pair.Key;
						FBanWordNode* Next = Pair.Value;

						FBanWordNode* Fail = Current->Failure;

						while (Fail != RootNode && !Fail->Children.Contains(Char))
						{
							Fail = Fail->Failure;
						}

						if (Fail->Children.Contains(Char) && Fail->Children[Char] != Next)
						{
							Next->Failure = Fail->Children[Char];
						}
						else
						{
							Next->Failure = RootNode;
						}

						// 핵심: 실패 노드가 끝이면 이어받기
						if (Next->Failure->bIsEnd)
						{
							Next->bIsEnd = true;
						}

						Queue.Enqueue(Next);
					}
				}
			}
		}
	}

	
}

void UBanWordFilterSubsystem::Deinitialize()
{
	Super::Deinitialize();

	DeleteNode(RootNode);
	RootNode = nullptr;
}

void UBanWordFilterSubsystem::DeleteNode(FBanWordNode* Node)
{
	if (!Node) return;

	for (auto& Pair : Node->Children)
	{
		DeleteNode(Pair.Value);
	}

	delete Node;
}
