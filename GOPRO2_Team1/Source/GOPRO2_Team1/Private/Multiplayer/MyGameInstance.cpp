// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>
#include "IdentityCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineIdentityInterface.h"


UMyGameInstance::UMyGameInstance()
{

	MySessionName = FName("My Session");

}


void UMyGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Init: No OnlineSubsystem found."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Init: OnlineSubsystem = %s"), *Subsystem->GetSubsystemName().ToString());

	// Identity interface
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (!Identity.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Init: Identity interface invalid."));
	}
	else
	{
		ELoginStatus::Type Status = Identity->GetLoginStatus(0);
		UE_LOG(LogTemp, Warning, TEXT("Init: Login status = %d (0=NotLoggedIn,1=UsingLocal,2=LoggedIn)"), (int)Status);
		bool bIsLoggedIn = Identity->GetUniquePlayerId(0).IsValid();
		UE_LOG(LogTemp, Warning, TEXT("Init: GetUniquePlayerId valid = %d"), bIsLoggedIn ? 1 : 0);
	}

	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		// Bind delegates here
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinSessionComplete);
	}

}


void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Game/Maps/Map1?Listen");
		}
	}
}

void UMyGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	SearchingForServer.Broadcast(false);

	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		int32 ArrayIndex = -1;
		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			++ArrayIndex;
			if (!Result.IsValid())
				continue;

			FServerInfo Info;
			FString ServerName = "Empy Server Name";
			FString HostName = "Empy Host Name";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);

			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.ServerArrayIndex = ArrayIndex;
			Info.SetPlayerCount();


			ServerListDel.Broadcast(Info);
		}

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UMyGameInstance::CreateSession(FString ServerName, FString HostName)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;

	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSettings.bIsLANMatch = false;
	else
		SessionSettings.bIsLANMatch = true; // IS LAN

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.NumPublicConnections = 2;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMyGameInstance::FindServers()
{
	SearchingForServer.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("JoinedServer"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSearch->bIsLanQuery = false; //IS NOT LAN
	else
		SessionSearch->bIsLanQuery = true; //IS LAN

	SessionSearch->MaxSearchResults = 500;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMyGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];

	if (Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JOINING SERVER AT INDEX: %d"), ArrayIndex);
		SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO JOIN SERVER AT INDEX: %d"), ArrayIndex);
	}
}

void UMyGameInstance::DestroyMySession()
{
	UE_LOG(LogTemp, Warning, TEXT("DestroyMySession called"));

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyMySession: SessionInterface invalid"));
		return;
	}

	DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnDestroySessionComplete);
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	bool bStarted = SessionInterface->DestroySession(MySessionName);
	UE_LOG(LogTemp, Warning, TEXT("DestroySession started: %d"), bStarted ? 1 : 0);
}

void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete: %s Success: %d"), *SessionName.ToString(), bWasSuccessful ? 1 : 0);

	if (!SessionInterface.IsValid())
		return;


	if (DestroySessionCompleteDelegateHandle.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		DestroySessionCompleteDelegateHandle.Reset();
	}

	if (bWasSuccessful)
	{

		UWorld* World = GetWorld();
		if (World)
		{

			World->ServerTravel(TEXT("/Game/Game/Menu?listen"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete reported failure for %s"), *SessionName.ToString());
	}
}