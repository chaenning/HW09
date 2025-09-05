#include "Game/CEGameModeBase.h"
#include "Game/CEGameStateBase.h"
#include "Player/CEPlayerController.h"
#include "EngineUtils.h"
#include "Player/CEPlayerState.h"

void ACEGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	/*ACEGameStateBase* CEGameStateBase =  GetGameState<ACEGameStateBase>();
	if (IsValid(CEGameStateBase) == true)
	{
		CEGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	}

	ACEPlayerController* CEPlayerController = Cast<ACEPlayerController>(NewPlayer);
	if (IsValid(CEPlayerController) == true)
	{
		AllPlayerControllers.Add(CEPlayerController);
	}*/
	ACEPlayerController* CEPlayerController = Cast<ACEPlayerController>(NewPlayer);
	if (IsValid(CEPlayerController) == true)
	{
		CEPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(CEPlayerController);

		ACEPlayerState* CEPS = CEPlayerController->GetPlayerState<ACEPlayerState>();
		if (IsValid(CEPS) == true)
		{
			CEPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACEGameStateBase* CEGameStateBase =  GetGameState<ACEGameStateBase>();
		if (IsValid(CEGameStateBase) == true)
		{
			CEGameStateBase->MulticastRPCBroadcastLoginMessage(CEPS->PlayerNameString);
		}
	}
}

FString ACEGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACEGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString ACEGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACEGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("SecretNumberString: %s"), *SecretNumberString)
}

void ACEGameModeBase::PrintChatMessageString(ACEPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{		
		IncreaseGuessCount(InChattingPlayerController);

		// 새로운 카운트로 메시지 다시 구성
		ACEPlayerState* CEPS = InChattingPlayerController->GetPlayerState<ACEPlayerState>();
		FString NewChatMessage;
		if (IsValid(CEPS))
		{
			NewChatMessage = CEPS->GetPlayerInfoString() + TEXT(": ") + GuessNumberString;
		}
		else
		{
			NewChatMessage = InChatMessageString;
		}
		
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
        
		for (TActorIterator<ACEPlayerController> It(GetWorld()); It; ++It)
		{
			ACEPlayerController* CEPlayerController = *It;
			if (IsValid(CEPlayerController) == true)
			{
				FString CombinedMessageString = NewChatMessage + TEXT(" -> ") + JudgeResultString;
				CEPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACEPlayerController> It(GetWorld()); It; ++It)
		{
			ACEPlayerController* CEPlayerController = *It;
			if (IsValid(CEPlayerController) == true)
			{
				CEPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ACEGameModeBase::IncreaseGuessCount(ACEPlayerController* InChattingPlayerController)
{
	ACEPlayerState* CEPS = InChattingPlayerController->GetPlayerState<ACEPlayerState>();
	if (IsValid(CEPS) == true)
	{
		CEPS->CurrentGuessCount++;
	}
}

void ACEGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CEPlayerController : AllPlayerControllers)
	{
		ACEPlayerState* CEPS = CEPlayerController->GetPlayerState<ACEPlayerState>();
		if (IsValid(CEPS) == true)
		{
			CEPS->CurrentGuessCount = 0;
		}
	}
}

void ACEGameModeBase::JudgeGame(ACEPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACEPlayerState* CEPS = InChattingPlayerController->GetPlayerState<ACEPlayerState>();
		for (const auto& CEPlayerController : AllPlayerControllers)
		{
			if (IsValid(CEPS) == true)
			{
				FString CombinedMessageString = CEPS->PlayerNameString + TEXT(" has won the game.");
				CEPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CEPlayerController : AllPlayerControllers)
		{
			ACEPlayerState* CEPS = CEPlayerController->GetPlayerState<ACEPlayerState>();
			if (IsValid(CEPS) == true)
			{
				if (CEPS->CurrentGuessCount < CEPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CEPlayerController : AllPlayerControllers)
			{
				CEPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
