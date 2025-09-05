#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CEGameModeBase.generated.h"

class ACEPlayerController;
/**
 * 
 */
UCLASS()
class CHATCE_API ACEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	
	void PrintChatMessageString(ACEPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ACEPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ACEPlayerController* InChattingPlayerController, int InStrikeCount);
protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACEPlayerController>> AllPlayerControllers;
};
