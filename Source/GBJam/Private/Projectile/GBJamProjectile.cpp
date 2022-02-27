// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/GBJamProjectile.h"

#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "Enemies/BaseEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/GBJamCharacter.h"


// Sets default values
AGBJamProjectile::AGBJamProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollider"));
	SetRootComponent(HitCollider);
	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AGBJamProjectile::HitOverlap);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void AGBJamProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTimeSpan);
}

void AGBJamProjectile::HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsPlayerFire)
	{
		if (const auto Enemy = Cast<ABaseEnemy>(OtherActor))
		{
			Enemy->ApplyDamage(Damage);
			Destroy();
		}
		else if (!Cast<AGBJamCharacter>(OtherActor) && !Cast<AGBJamProjectile>(OtherActor))
		{
			Destroy();
		}
		
	}
	else
	{
		if (const auto Player = Cast<AGBJamCharacter>(OtherActor))
		{
			Player->ApplyDamage(Damage);
			Destroy();
		}
		else if (!Cast<ABaseEnemy>(OtherActor) && !Cast<AGBJamProjectile>(OtherActor))
		{
			Destroy();
		}
	}
}

// Called every frame
void AGBJamProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGBJamProjectile::SetDamage(int32 NewDamage)
{
	Damage = NewDamage;
}