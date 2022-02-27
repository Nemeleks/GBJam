// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ToolsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GBJAM_API UToolsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	void AddEnemySpawner(class AEnemySpawner* EnemySpawner);
	void SpawnAllEnemies();
	
	void AddCheckPoint(class ACheckPoint* CheckPoint);
	void DeactivateAllCheckPoints();

	void AddEnemy(class ABaseEnemy* Enemy);
	void SetEnemiesMaxHealth();
	void RemoveEnemy(ABaseEnemy* Enemy);

private:
	TArray<AEnemySpawner*> EnemySpawners;
	TArray<ACheckPoint*> CheckPoints;
	TArray<ABaseEnemy*> Enemies;
	
};

