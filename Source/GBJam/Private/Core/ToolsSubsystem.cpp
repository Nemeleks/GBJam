// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ToolsSubsystem.h"

#include "Enemies/EnemySpawner.h"
#include "Tools/CheckPoint.h"

void UToolsSubsystem::AddEnemySpawner(AEnemySpawner* EnemySpawner)
{
	EnemySpawners.Add(EnemySpawner);
}

void UToolsSubsystem::SpawnAllEnemies()
{
	for ( const auto Spawner : EnemySpawners)
	{
		Spawner->SpawnEnemy();
	}
}

void UToolsSubsystem::AddCheckPoint(ACheckPoint* CheckPoint)
{
	CheckPoints.Add(CheckPoint);
}


void UToolsSubsystem::DeactivateAllCheckPoints()
{
	for (const auto CheckPoint : CheckPoints)
	{
		CheckPoint->SetIsActivated(false);
	}
}