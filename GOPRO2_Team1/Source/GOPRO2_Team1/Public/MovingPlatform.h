// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "MovingPlatform.generated.h"

UCLASS()
class GOPRO2_TEAM1_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMovingPlatform();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Movement")
	TArray<FVector> TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Movement")
	float MoveSpeed = 100.0f;

	/* Replication */

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedLocation, VisibleAnywhere, Category = "Replication")
	FVector ReplicatedLocation;

	UFUNCTION()
	void OnRep_ReplicatedLocation();

	TArray<FVector> PathPoints;

	int32 CurrentTargetIndex;

	int32 MovementDirection;
	

public:	
	
	virtual void Tick(float DeltaTime) override;

	void ServerMovePlatform(float DeltaTime);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
