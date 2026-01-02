// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyGameStateBase.h"
#include "Multiplayer/MyGameInstance.h"
#include "GameFramework/PlayerState.h"

void AMyPlayerController::Server_ToggleReady_Implementation(bool bClientReady) 
{
	AMyGameStateBase* GS = GetWorld()->GetGameState<AMyGameStateBase>();
	if (!GS) return;

	if (GS->PlayerArray.Num() > 0 && GS->PlayerArray[0] == this->PlayerState)
	{
		GS->bPlayer1Ready = bClientReady;
		UE_LOG(LogTemp, Warning, TEXT("Host is now %s"), bClientReady ? TEXT("READY") : TEXT("NOT READY"))
	}
	else
	{
		GS->bPlayer2Ready = bClientReady;
		UE_LOG(LogTemp, Warning, TEXT("Client is now %s"), bClientReady ? TEXT("READY") : TEXT("NOT READY"));
	}

	GS->CheckAllReady();
}

void AMyPlayerController::HandleReturnToMainMenu()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	
	if (GI)
	{
		GI->DestroyMySession();

		ClientReturnToMainMenuWithTextReason(FText::FromString("You left the session."));
	}
}

