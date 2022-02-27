// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scorpion.h"
#include "GameFramework/Character.h"
#include "Boss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDie);

UCLASS()
class GBJAM_API ABoss : public AScorpion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* HitCollider;

	virtual void OnDeath() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UArrowComponent* GetProjectileSpawnPoint() const {return ProjectileSpawnPoint;}

	

	FOnBossDie OnBossDie;
};
