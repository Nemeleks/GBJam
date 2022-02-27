// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/BossSpawner.h"

#include "AI/BossAIController.h"
#include "Components/ArrowComponent.h"
#include "Enemies/Boss.h"
#include "Tools/Door.h"


// Sets default values
ABossSpawner::ABossSpawner()
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
void ABossSpawner::BeginPlay()
{
	Super::BeginPlay();
	//SpawnEnemy();
}

// Called every frame
void ABossSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossSpawner::SpawnEnemy()
{
	if (SpawnedBoss)
	{
		SpawnedBoss->Destroy();
		SpawnedBoss = nullptr;
	}
	if (BossClass)
	{
		const FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		const FRotator SpawnRotation = SpawnPoint->GetComponentRotation();
		SpawnedBoss = GetWorld()->SpawnActor<ABoss>(BossClass, SpawnLocation, SpawnRotation);
		if (SpawnedBoss)
		{
			SpawnedBoss->OnBossDie.AddDynamic(this, &ThisClass::OpenDoor);
			const auto AIController = Cast<ABossAIController>(SpawnedBoss->GetController());
			if (AIController)
			{
				AIController->SetControlledPawn(SpawnedBoss);
			}
		}
	}
}

void ABossSpawner::OpenDoor()
{
	if (Door)
	{
		Door->Open();
	}
}
