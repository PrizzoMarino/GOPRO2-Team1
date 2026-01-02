// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GOPRO2_TEAM1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Multiplayer")
	void Server_ToggleReady(bool bClientready);

	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
	void HandleReturnToMainMenu();
	
};
