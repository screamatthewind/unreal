// Copyright Epic Games, Inc. All Rights Reserved.

#include "UsingCamerasGameMode.h"
#include "UsingCamerasCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUsingCamerasGameMode::AUsingCamerasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
