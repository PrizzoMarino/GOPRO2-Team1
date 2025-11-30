// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class GOPRO2_TEAM1_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMovingPlatform();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing", meta = (ExposeOnSpawn = "true", MakeEditWidget = "true"))
	TArray<FVector> ThePath;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components") class UBoxComponent* BoxCollider;
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components") class UInterpToMovementComponent* MovementComponent;

};
