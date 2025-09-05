// Fill out your copyright notice in the Description page of Project Settings.


#include "player/CEPawn.h"

#include "ChatCE/ChatCE.h"

// Sets default values
ACEPawn::ACEPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACEPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetRoleString = ChatCEFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *ChatCEFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatCEFunctionLibrary::MyPrintString(this, CombinedString, 10.f);	
}

void ACEPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = ChatCEFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *ChatCEFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatCEFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
