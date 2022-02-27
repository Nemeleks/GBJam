// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Boss.h"

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ABoss::ABoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugLine(GetWorld(), GetProjectileSpawnPoint()->GetComponentLocation(),
		GetProjectileSpawnPoint()->GetComponentLocation()+ GetProjectileSpawnPoint()->GetForwardVector()*3000.f,
		FColor::Red);

	DrawDebugLine(GetWorld(), GetProjectileSpawnPoint()->GetComponentLocation(),
		GetProjectileSpawnPoint()->GetComponentLocation()+ GetProjectileSpawnPoint()->GetUpVector()*3000.f,
		FColor::Blue);

	DrawDebugLine(GetWorld(), GetActorLocation(),
		GetActorLocation()+ GetActorForwardVector()*3000.f,
		FColor::Green);
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

