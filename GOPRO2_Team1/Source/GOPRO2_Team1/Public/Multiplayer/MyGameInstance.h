// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString ServerName;
	UPROPERTY(BlueprintReadOnly)
		FString PlayerCountStr;
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 MaxPlayers;

	void SetPlayerCount() 
	{
		PlayerCountStr = FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);

UCLASS()
class GOPRO2_TEAM1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UMyGameInstance();

protected:

	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	
	UFUNCTION(BlueprintCallable)
	void CreateSession(FString ServerName, FString HostName);
	
	UFUNCTION(BlueprintCallable)
	void FindServers();
	
};
