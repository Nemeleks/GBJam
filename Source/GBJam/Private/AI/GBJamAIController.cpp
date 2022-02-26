// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GBJamAIController.h"

#include "DrawDebugHelpers.h"
#include "Enemies/BaseEnemy.h"
#include "Player/GBJamCharacter.h"

void AGBJamAIController::BeginPlay()
{
	Super::BeginPlay();

	const auto EnemyPawn = Cast<ABaseEnemy>(GetPawn());
	if (EnemyPawn)
	{
		ControlledPawn = EnemyPawn;
	}
	const auto PlayerCharacter = Cast<AGBJamCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter)
	{
		Player = PlayerCharacter;
	}
}

void AGBJamAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ControlledPawn && Player)
	{
		Targeting();
	}
	
}

void AGBJamAIController::Targeting()
{

	if (FVector::DistSquared(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) <= FMath::Square(ControlledPawn->GetAttackRange()))
	{
		RotateToPlayer();
		if (Player->GetIsAlive() && ControlledPawn->GetIsAlive())
		{
			ControlledPawn->Attack();
		}
		
	}
	else if(FVector::DistSquared(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) <= FMath::Square(ControlledPawn->GetTargetingRange()))
	{
		RotateToPlayer();
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), 1);
	}

	
}

void AGBJamAIController::RotateToPlayer()
{
	const FVector PawnLocation = ControlledPawn->GetActorLocation();
	const FVector ForwardDirection = ControlledPawn->GetActorForwardVector();
	FVector MoveDirection = Player->GetActorLocation() - PawnLocation;
	MoveDirection.Normalize();
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	if (ForwardAngle >= 170.f)
	{
		UE_LOG(LogTemp, Error, TEXT("AddYaw"));
		ControlledPawn->SetActorRotation(ControlledPawn->GetActorRotation() + FRotator(0.f,180.f,0.f));
	}
}
