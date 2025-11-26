// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void AMovingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingPlatform, ReplicatedLocation);
}



// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	NetUpdateFrequency = 60.0f;

	CurrentTargetIndex = 1;
	MovementDirection = 1;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		FVector InitialLocation = GetActorLocation();

		PathPoints.Empty();
		PathPoints.Add(InitialLocation);

		PathPoints.Append(TargetLocation);

		ReplicatedLocation = InitialLocation;

		if (PathPoints.Num() <= 1) 
		{
			PrimaryActorTick.bCanEverTick = false;
		}
	}
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) 
	{
		ServerMovePlatform(DeltaTime);
	}
	else 
	{
		FVector CurrentLocation = GetActorLocation();
		SetActorLocation(FMath::VInterpTo(CurrentLocation, ReplicatedLocation, DeltaTime, 100.0f));
	}

}

void AMovingPlatform::ServerMovePlatform(float DeltaTime) 
{
	if (PathPoints.Num() < 2) 
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector NextTargetPoint = PathPoints[CurrentTargetIndex];

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, NextTargetPoint, DeltaTime, MoveSpeed);
	SetActorLocation(NewLocation);

	ReplicatedLocation = NewLocation;

	if (FVector::DistSquared(NewLocation, NextTargetPoint) < 1.0f) 
	{
		CurrentTargetIndex += MovementDirection;

		if (CurrentTargetIndex >= PathPoints.Num() - 1) 
		{
			MovementDirection = -1;
			CurrentTargetIndex = PathPoints.Num() - 1;

			CurrentTargetIndex += MovementDirection;
		}

		else if (CurrentTargetIndex <=0)
		{
			MovementDirection = 1;
			CurrentTargetIndex = 0;

			CurrentTargetIndex += MovementDirection;
		}
	}
}

void AMovingPlatform::OnRep_ReplicatedLocation() 
{
	FVector CurrentLocation = GetActorLocation();

	float DistanceSquared = FVector::DistSquared(CurrentLocation, ReplicatedLocation);

	if (DistanceSquared > FMath::Square(100.0f)) 
	{
		SetActorLocation(ReplicatedLocation);
	}
}

