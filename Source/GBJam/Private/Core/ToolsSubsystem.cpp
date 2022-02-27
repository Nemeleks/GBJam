// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ToolsSubsystem.h"

#include "Enemies/BaseEnemy.h"
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

void UToolsSubsystem::AddEnemy(ABaseEnemy* Enemy)
{
	Enemies.AddUnique(Enemy);
}

void UToolsSubsystem::SetEnemiesMaxHealth()
{
	for	(const auto Enemy : Enemies)
	{
		Enemy->SetMaxHP();
	}
}

void UToolsSubsystem::RemoveEnemy(ABaseEnemy* Enemy)
{
	if (Enemies.Contains(Enemy))
	{
		Enemies.Remove(Enemy);
	}
}
