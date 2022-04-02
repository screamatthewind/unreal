// Copyright Epic Games, Inc. All Rights Reserved.

#include "orbit1GameMode.h"
#include "orbit1Pawn.h"

Aorbit1GameMode::Aorbit1GameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = Aorbit1Pawn::StaticClass();
}
