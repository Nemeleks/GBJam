// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/GBJamGameMode.h"
#include "Player/GBJamCharacter.h"

AGBJamGameMode::AGBJamGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGBJamCharacter::StaticClass();	
}
