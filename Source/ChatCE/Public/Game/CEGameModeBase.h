#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CEGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CHATCE_API ACEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;


};
