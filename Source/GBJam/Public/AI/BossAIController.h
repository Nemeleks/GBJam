// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BossAIController.generated.h"

UCLASS()
class GBJAM_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetControlledPawn(class ABoss* Enemy) { ControlledPawn = Enemy;}

private:
	UPROPERTY()
	class ABoss* ControlledPawn;

	UPROPERTY()
	class AGBJamCharacter* Player;

	void Targeting();
	void RotateToPlayer();
	float TargetingSpeed = 100.f;

	void SetNewDirection();
	void ResetDirection();
	bool bCanChooseNewDirection = true;
	bool bCanJump = false;
	float MovementValue;
	float ResetDirectionRate;
	FTimerHandle ResetDirectionTimerHandle;
	
};
