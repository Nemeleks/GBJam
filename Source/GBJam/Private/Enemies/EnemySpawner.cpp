// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySpawner.h"

#include "AI/GBJamAIController.h"
#include "Components/ArrowComponent.h"
#include "Core/ToolsSubsystem.h"
#include "Enemies/BaseEnemy.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		SetRootComponent(SceneComponent);
	}

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy();
	
	if (const auto Subsystem = GetWorld()->GetSubsystem<UToolsSubsystem>())
	{
		Subsystem->AddEnemySpawner(this);
	}
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy()
{
	if (SpawnedEnemy)
	{
		SpawnedEnemy->Destroy();
		SpawnedEnemy = nullptr;
	}
	if (EnemyClass)
	{
		const FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		const FRotator SpawnRotation = SpawnPoint->GetComponentRotation();
		SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyClass, SpawnLocation, SpawnRotation);
		if (SpawnedEnemy)
		{
			const auto AIController = Cast<AGBJamAIController>(SpawnedEnemy->GetController());
			if (AIController)
			{
				AIController->SetControlledPawn(SpawnedEnemy);
			}
		}
	}
}