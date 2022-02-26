// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GameFramework/Character.h"
#include "Slug.generated.h"

UCLASS()
class GBJAM_API ASlug : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlug();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* AttackDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USphereComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float LaunchForce = 100.f;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	virtual void StopAttacking() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION()
	//void OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FTimerHandle AttackRateTimerHandle;
};
