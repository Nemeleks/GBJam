// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSpawner.generated.h"

UCLASS()
class GBJAM_API ABossSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossSpawner();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	class UArrowComponent* SpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<class ABoss> BossClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	class ADoor* Door;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();

private:
	UPROPERTY()
	ABoss* SpawnedBoss;

	UFUNCTION()
	void OpenDoor();
};
