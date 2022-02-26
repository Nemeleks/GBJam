// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Interfaces/Damageable.h"
#include "BaseEnemy.generated.h"

UCLASS(Abstract)
class GBJAM_API ABaseEnemy : public APaperCharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destroying")
	float DestroyingCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 HitDamage = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ChangeColorRate = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float TargetingRange = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRate = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* RunAnimation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* AttackAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* JumpAnimation;

	void UpdateAnimation();

	void UpdatePawn();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	bool bIsAttacking = false;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ApplyDamage(int32 DamageAmount) override;

	float GetTargetingRange() const {return TargetingRange;}
	float GetAttackRange() const {return AttackRange;}
	virtual void Attack();
	virtual void StopAttacking();
	bool GetIsAlive() const {return bIsAlive;}

private:

	FTimerHandle DestroyTimerHandle;
	FTimerHandle ChangeSpriteColorTimerHandle;
	
	bool bIsAlive = true;

	void DestroyPawn();
	void ResetSpriteColor();

	float PrevDirection;
	
};


