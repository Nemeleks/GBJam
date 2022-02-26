// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "AIController.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Player/GBJamCharacter.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ABaseEnemy::HitOverlap);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePawn();
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::ApplyDamage(int32 DamageAmount)
{
	if (HealthComponent && bIsAlive)
	{
		HealthComponent->TakeDamage(DamageAmount);
		GetSprite()->SetSpriteColor(FLinearColor::Red);
		GetWorld()->GetTimerManager().SetTimer(ChangeSpriteColorTimerHandle, this, &ThisClass::ResetSpriteColor, ChangeColorRate);
	}
}

void ABaseEnemy::DestroyPawn()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
	Destroy();
}

void ABaseEnemy::StopAttacking()
{

}

void ABaseEnemy::ResetSpriteColor()
{
	GetSprite()->SetSpriteColor(FLinearColor::White);
	GetWorld()->GetTimerManager().ClearTimer(ChangeSpriteColorTimerHandle);
}

void ABaseEnemy::UpdateAnimation()
{
	if (!bIsAlive)
	{
		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(DeathAnimation);
	}
	else if (bIsAttacking && AttackAnimation)
	{
		GetSprite()->SetFlipbook(AttackAnimation);
	}

	else if (GetMovementComponent()->IsFalling())
	{
		if (JumpAnimation)
		{
			GetSprite()->SetFlipbook(JumpAnimation);
		}
	}
	
	else
	{
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunAnimation : IdleAnimation;
		if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
	}
}

void ABaseEnemy::UpdatePawn()
{
	UpdateAnimation();

	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	if (FMath::Sign(TravelDirection) != FMath::Sign(PrevDirection))
	{
		if (TravelDirection < 0.0f)
		{
			SetActorRotation(FRotator(0.f,0.f,0.f));
		}
		else if (TravelDirection > 0.0f)
		{
			SetActorRotation(FRotator(0.f,180.f,0.f));
		}
	}
	PrevDirection = TravelDirection;
}

void ABaseEnemy::OnDeath()
{
	bIsAlive = false;
	UnPossessed();
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::DestroyPawn, DestroyingCooldown);
}

void ABaseEnemy::HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Player = Cast<AGBJamCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player && OtherActor == Player && OtherComp == Player->GetCollider())
	{

		Player->ApplyDamage(HitDamage);
	}
}

void ABaseEnemy::Attack()
{
	if (!AttackAnimation)
	{
		return;
	}
	
}
