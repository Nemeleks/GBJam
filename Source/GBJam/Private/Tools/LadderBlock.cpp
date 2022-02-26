// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LadderBlock.h"


// Sets default values
ALadderBlock::ALadderBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALadderBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadderBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

