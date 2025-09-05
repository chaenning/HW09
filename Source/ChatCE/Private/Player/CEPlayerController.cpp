#include "Player/CEPlayerController.h"

#include "EngineUtils.h"
#include "ChatCE/ChatCE.h"
#include "UI/CEChatInput.h"
#include "Kismet/KismetSystemLibrary.h"

void ACEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UCEChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ACEPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);		
	}	
}

void ACEPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Red, 5.0f);

	/*FString NetModeString = ChatCEFunctionLibrary::GetNetModeString(this);
	FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	ChatCEFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);*/

	ChatCEFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ACEPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
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

void ACEPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
