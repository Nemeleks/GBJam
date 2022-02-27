// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/GBJamGameMode.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Player/GBJamCharacter.h"

AGBJamGameMode::AGBJamGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGBJamCharacter::StaticClass();	
}

void AGBJamGameMode::BeginPlay()
{
	Super::BeginPlay();

	const auto Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		Controller->SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
	}
}
