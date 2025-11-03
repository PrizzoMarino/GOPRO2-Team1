// Copyright Epic Games, Inc. All Rights Reserved.

#include "GOPRO2_Team1GameMode.h"
#include "GOPRO2_Team1Character.h"
#include "UObject/ConstructorHelpers.h"

AGOPRO2_Team1GameMode::AGOPRO2_Team1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
