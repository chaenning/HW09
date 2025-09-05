#include "Game/CEGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CEPlayerController.h"

void ACEGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACEPlayerController* CEPC = Cast<ACEPlayerController>(PC);
			if (IsValid(CEPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CEPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
