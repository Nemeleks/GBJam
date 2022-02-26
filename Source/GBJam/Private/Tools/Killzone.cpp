// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Killzone.h"

#include "Components/BoxComponent.h"
#include "Interfaces/Damageable.h"


// Sets default values
AKillzone::AKillzone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapFunc);
}

// Called when the game starts or when spawned
void AKillzone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKillzone::OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto Actor = Cast<IDamageable>(OtherActor))
	{
		Actor->ApplyDamage(Damage);
	}
}

// Called every frame
void AKillzone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

