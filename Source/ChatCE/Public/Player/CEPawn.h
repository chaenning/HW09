// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CEPawn.generated.h"

UCLASS()
class CHATCE_API ACEPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACEPawn();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
