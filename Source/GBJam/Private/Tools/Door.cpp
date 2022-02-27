// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Door.h"

#include "PaperSpriteComponent.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::Open()
{
	Sprite->SetSpriteColor(FLinearColor::Red);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ADoor::OpenDestroy, DestroyDelay);
}

void ADoor::OpenDestroy()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
	Destroy();
}

