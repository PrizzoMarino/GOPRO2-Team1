// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameStateBase, bPlayer1Ready);
	DOREPLIFETIME(AMyGameStateBase, bPlayer2Ready);

}

void AMyGameStateBase::CheckAllReady()
{
	if (HasAuthority() && bPlayer1Ready && bPlayer2Ready)
	{
		GetWorld()->ServerTravel(TEXT("Game/Game/Maps/CharacterSelection?Listen"));
	}
}

