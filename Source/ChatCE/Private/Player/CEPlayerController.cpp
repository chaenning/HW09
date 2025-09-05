#include "Player/CEPlayerController.h"

#include "EngineUtils.h"
#include "ChatCE/ChatCE.h"
#include "Game/CEGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CEChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/CEPlayerState.h"

ACEPlayerController::ACEPlayerController()
{
	bReplicates = true;
}

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

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ACEPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessageString);
		ACEPlayerState* CEPS = GetPlayerState<ACEPlayerState>();
		if (IsValid(CEPS) == true)
		{
			//FString CombinedMessageString = CEPS->PlayerNameString + TEXT(": ") + InChatMessageString;
			FString CombinedMessageString = CEPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}		
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

void ACEPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ACEPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	/*for (TActorIterator<ACEPlayerController> It(GetWorld()); It; ++It)
	{
		ACEPlayerController* CEPlayerController = *It;
		if (IsValid(CEPlayerController) == true)
		{
			CEPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}*/
	
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ACEGameModeBase* CEGM = Cast<ACEGameModeBase>(GM);
		if (IsValid(CEGM) == true)
		{
			CEGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ACEPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}
