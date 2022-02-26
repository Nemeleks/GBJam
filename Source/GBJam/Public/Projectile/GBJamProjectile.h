// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GBJamProjectile.generated.h"

UCLASS()
class GBJAM_API AGBJamProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGBJamProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USphereComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* Sprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	bool bIsPlayerFire = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float LifeTimeSpan = 5.f;

	UFUNCTION()
	void HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int32 NewDamage);

private:
	int32 Damage = 1;
};


