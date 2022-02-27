// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class GBJAM_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UPaperSpriteComponent* Sprite;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	float DestroyDelay = 1.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Open();

private:
	FTimerHandle DestroyTimerHandle;
	void OpenDestroy();
};
