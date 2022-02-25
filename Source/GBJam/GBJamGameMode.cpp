// Copyright Epic Games, Inc. All Rights Reserved.

#include "GBJamGameMode.h"
#include "GBJamCharacter.h"

AGBJamGameMode::AGBJamGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGBJamCharacter::StaticClass();	
}
