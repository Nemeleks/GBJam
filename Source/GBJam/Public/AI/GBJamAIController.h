// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GBJamAIController.generated.h"

/**
 * 
 */
UCLASS()
class GBJAM_API AGBJamAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	class ABaseEnemy* ControlledPawn;

	UPROPERTY()
	class AGBJamCharacter* Player;

	void Targeting();
	void RotateToPlayer();
	
};
