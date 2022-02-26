// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Scorpion.h"

#include "Components/ArrowComponent.h"
#include "Projectile/GBJamProjectile.h"


// Sets default values
AScorpion::AScorpion()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AScorpion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScorpion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScorpion::Attack()
{
	if (!ProjectileClass)
	{
		return;;
	}
	Super::Attack();

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		const FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		const FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		const auto Projectile = GetWorld()->SpawnActor<AGBJamProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		if (Projectile)
		{
			Projectile->SetDamage(HitDamage);
		}
		GetWorld()->GetTimerManager().SetTimer(AttackRateTimerHandle, this, &ThisClass::StopAttacking, AttackRate);
		
	}
}

void AScorpion::StopAttacking()
{
	Super::StopAttacking();
	bIsAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackRateTimerHandle);
}

// Called to bind functionality to input
void AScorpion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

