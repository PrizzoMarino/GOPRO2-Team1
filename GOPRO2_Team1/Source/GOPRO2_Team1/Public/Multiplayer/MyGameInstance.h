// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOPRO2_TEAM1_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UMyGameInstance();

protected:
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void CreateSession();
	
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void JoinSession();
	
};
