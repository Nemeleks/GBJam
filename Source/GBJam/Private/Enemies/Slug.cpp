// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Slug.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
ASlug::ASlug()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackDirection"));

	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);
	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HitOverlap);
}

// Called when the game starts or when spawned
void ASlug::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlug::Attack()
{
	Super::Attack();

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		
		Jump();
		//LaunchCharacter(AttackDirection->GetForwardVector(), false, false);

		GetWorld()->GetTimerManager().SetTimer(AttackRateTimerHandle, this, &ThisClass::StopAttacking, AttackRate);
		
	}
}

void ASlug::StopAttacking()
{
	Super::StopAttacking();

	bIsAttacking = false;
	
	GetWorld()->GetTimerManager().ClearTimer(AttackRateTimerHandle);
	
}

// Called to bind functionality to input
void ASlug::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

