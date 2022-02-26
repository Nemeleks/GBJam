// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GBJamAIController.h"

#include "DrawDebugHelpers.h"
#include "Enemys/BaseEnemy.h"

void AGBJamAIController::BeginPlay()
{
	Super::BeginPlay();

	const auto EnemyPawn = Cast<ABaseEnemy>(GetPawn());
	if (EnemyPawn)
	{
		ControlledPawn = EnemyPawn;
	}
	const auto PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (PlayerCharacter)
	{
		Player = PlayerCharacter;
	}
}

void AGBJamAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ControlledPawn)
	{
		ControlledPawn->AddMovementInput(FVector(1.f,0.f,0.f), 1);
	}
	
}
