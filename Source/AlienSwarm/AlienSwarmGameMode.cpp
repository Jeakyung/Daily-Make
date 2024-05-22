// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienSwarmGameMode.h"
#include "AlienSwarmCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlienSwarmGameMode::AAlienSwarmGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
