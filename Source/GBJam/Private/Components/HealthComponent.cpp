// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(int32 Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage,0, MaxHealth);
	
	if (CurrentHealth == 0)
	{
		// ToDo EndGame
		OnDeath.Broadcast();
	}
	
}

void UHealthComponent::SetMaxHP()
{
	CurrentHealth = MaxHealth;
}

