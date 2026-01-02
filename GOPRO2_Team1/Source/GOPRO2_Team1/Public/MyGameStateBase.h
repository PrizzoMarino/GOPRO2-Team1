// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GOPRO2_TEAM1_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Multiplayer")
    bool bPlayer1Ready = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Multiplayer")
    bool bPlayer2Ready = false;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void CheckAllReady();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
