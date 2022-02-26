// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class GBJAM_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* ActivationCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UPaperFlipbookComponent* Sprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* RespawnPoint;

	UFUNCTION()
	void OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetRespawnPoint() const {return GetActorLocation();}

	void Activate();

private:
	bool bIsActivated = false;
};


