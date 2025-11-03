// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"
#include "Interfaces/OnlineSessionInterface.h"

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
	
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
		void CreateSession();

	UFUNCTION(BlueprintCallable)
		void JoinSession();
	
	
};
