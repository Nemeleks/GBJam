// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossActivator.generated.h"

UCLASS()
class GBJAM_API ABossActivator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossActivator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	class ABossSpawner* BossSpawner;

	UFUNCTION()
	void OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};


