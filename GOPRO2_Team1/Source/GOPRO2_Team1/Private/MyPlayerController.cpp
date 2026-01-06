// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyGameStateBase.h"
#include "Multiplayer/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"

void AMyPlayerController::Server_ToggleReady_Implementation()
{
	AMyGameStateBase* GS = GetWorld()->GetGameState<AMyGameStateBase>();
	if (!GS) return;

	if (GS->PlayerArray.Num() > 0 && GS->PlayerArray[0] == this->PlayerState)
	{
		GS->bPlayer1Ready = !GS->bPlayer1Ready;
		UE_LOG(LogTemp, Warning, TEXT("Host toggle: %s"), GS->bPlayer1Ready ? TEXT("READY") : TEXT("NOT READY"))
	}
	else if (GS->PlayerArray.Num() > 1 && GS->PlayerArray[1] == this->PlayerState)
	{
		GS->bPlayer2Ready = !GS->bPlayer2Ready;
		UE_LOG(LogTemp, Warning, TEXT("Client toggle: %s"), GS->bPlayer2Ready ? TEXT("READY") : TEXT("NOT READY"));
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

void AMyPlayerController::ClosePauseMenu()
{
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		// SetPause(false); 
	}
}

