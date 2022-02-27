// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/BossActivator.h"

#include "Components/BoxComponent.h"
#include "Tools/BossSpawner.h"


// Sets default values
ABossActivator::ABossActivator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapFunc);
}

void ABossActivator::OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (BossSpawner)
	{
		if (GetWorld()->GetFirstPlayerController()->GetPawn() == OtherActor)
		{
			BossSpawner->SpawnEnemy();
		}
	}
}

// Called when the game starts or when spawned
void ABossActivator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossActivator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

