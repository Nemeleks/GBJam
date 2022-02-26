// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GameFramework/Character.h"
#include "Scorpion.generated.h"

UCLASS()
class GBJAM_API AScorpion : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AScorpion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AGBJamProjectile> ProjectileClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	virtual void StopAttacking() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle AttackRateTimerHandle;

	bool bCanFire = true;
};
