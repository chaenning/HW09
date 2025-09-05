// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CEPlayerController.generated.h"

class UCEChatInput;

UCLASS()
class CHATCE_API ACEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCEChatInput> ChatInputWidgetClass;
	UPROPERTY()
	TObjectPtr<UCEChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
};
