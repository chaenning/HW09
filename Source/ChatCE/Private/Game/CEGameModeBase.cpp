#include "Game/CEGameModeBase.h"

#include "Game/CEGameStateBase.h"

void ACEGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACEGameStateBase* CEGameStateBase =  GetGameState<ACEGameStateBase>();
	if (IsValid(CEGameStateBase) == true)
	{
		CEGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	}
}