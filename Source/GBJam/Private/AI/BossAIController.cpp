// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAIController.h"

#include "Components/ArrowComponent.h"
#include "Enemies/Boss.h"
#include "Player/GBJamCharacter.h"


void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	const auto EnemyPawn = Cast<ABoss>(GetPawn());
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

void ABossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ControlledPawn && Player)
	{
		Targeting();
	}

	if (bCanChooseNewDirection)
	{
		SetNewDirection();
	}
	
}

void ABossAIController::Targeting()
{

	if (FVector::DistSquared(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) <= FMath::Square(ControlledPawn->GetAttackRange()))
	{
		
		if (Player->GetIsAlive() && ControlledPawn->GetIsAlive())
		{
			RotateToPlayer();
			ControlledPawn->Attack();
			if (bCanJump)
			{
				ControlledPawn->Jump();
			}
			ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementValue);
		}
		
	}
	else if(FVector::DistSquared(ControlledPawn->GetActorLocation(), Player->GetActorLocation()) <= FMath::Square(ControlledPawn->GetTargetingRange()))
	{

		if (Player->GetIsAlive() && ControlledPawn->GetIsAlive())
		{
			RotateToPlayer();
			ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), 1);
		}

	}

	
}

void ABossAIController::RotateToPlayer()
{
	const FVector PawnLocation = ControlledPawn->GetActorLocation();
	const FVector ForwardDirection = ControlledPawn->GetActorForwardVector();
	FVector MoveDirection = Player->GetActorLocation() - PawnLocation;
	MoveDirection.Normalize();
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	if (ForwardAngle >= 90.f)
	{
		//UE_LOG(LogTemp, Error, TEXT("AddYaw"));
		ControlledPawn->SetActorRotation(ControlledPawn->GetActorRotation() + FRotator(0.f,180.f,0.f));
	}
	
	const FVector ProjectileSpawnPointForwardDirection = ControlledPawn->GetProjectileSpawnPoint()->GetForwardVector();
	const FVector ProjectileSpawnPointUpDirection = ControlledPawn->GetProjectileSpawnPoint()->GetUpVector();
	FVector TargetingDirection = Player->GetActorLocation() - ControlledPawn->GetProjectileSpawnPoint()->GetComponentLocation();
	TargetingDirection.Normalize();
	
	float TargetingForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ProjectileSpawnPointForwardDirection, TargetingDirection)));
	float TargetingUpAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ProjectileSpawnPointUpDirection, TargetingDirection)));

	UE_LOG(LogTemp, Error, TEXT("%f"), ForwardAngle);
	if (TargetingForwardAngle > 5.f && TargetingUpAngle < 90.f)
	{
		ControlledPawn->GetProjectileSpawnPoint()->SetWorldRotation(ControlledPawn->GetProjectileSpawnPoint()->GetComponentRotation() + FRotator(1.f,0.f,0.f)*GetWorld()->GetDeltaSeconds()*TargetingSpeed);
	}
	else if (TargetingForwardAngle > 5.f && TargetingUpAngle > 90.f)
	{
		ControlledPawn->GetProjectileSpawnPoint()->SetWorldRotation(ControlledPawn->GetProjectileSpawnPoint()->GetComponentRotation() + FRotator(-1.f,0.f,0.f)*GetWorld()->GetDeltaSeconds()*TargetingSpeed);
	}
	
}

void ABossAIController::SetNewDirection()
{
	bCanChooseNewDirection = false;
	bCanJump = FMath::RandBool();
	MovementValue = FMath::RandRange(-1, 1);
	ResetDirectionRate = FMath::RandRange(1, 3);
	GetWorld()->GetTimerManager().SetTimer(ResetDirectionTimerHandle, this, &ABossAIController::ResetDirection, ResetDirectionRate);
}

void ABossAIController::ResetDirection()
{
	GetWorld()->GetTimerManager().ClearTimer(ResetDirectionTimerHandle);
	bCanChooseNewDirection = true;
}
