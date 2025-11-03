// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMyGameInstance::UMyGameInstance()
{
	
}

void UMyGameInstance::Init()
{
	
	if(IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSub->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind session delegates here if needed
			//Sessioninterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::)
		}
	}
	
}


void UMyGameInstance::CreateSession()
{
}


void UMyGameInstance::JoinSession()
{
}
