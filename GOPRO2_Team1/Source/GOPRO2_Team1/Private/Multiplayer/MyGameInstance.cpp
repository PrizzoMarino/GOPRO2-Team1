// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>
#include "GameplayStatics.generated.h"
#include "IdentityCommon.h"
#include "Interfaces/OnlineIdentityInterface.h"

UMyGameInstance::UMyGameInstance()
{

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


void UMyGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded) 
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeeded);
	if(Succeeded)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/Game/Maps/Test?Listen");
		}
	}
}

void UMyGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		TArray <FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SearchResults.Num());
		
		if (SearchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("JoinServer"));
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}

	}
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));
	if (SessionInterface.IsValid())
	{
		FString Address;
		if (SessionInterface->GetResolvedConnectString(SessionName, Address))
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Address: %s"), *Address);
			APlayerController* PlayerController = GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMyGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}

void UMyGameInstance::JoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinedServer"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false; //IS LAN
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

