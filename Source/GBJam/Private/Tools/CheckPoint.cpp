// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/CheckPoint.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Player/GBJamCharacter.h"


// Sets default values
ACheckPoint::ACheckPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivationCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ActivationCollider);
	ActivationCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapFunc);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	RespawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("RespawnPoint"));
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	Sprite->SetSpriteColor(FLinearColor::Red);
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckPoint::Activate()
{
	if (!bIsActivated)
	{
		bIsActivated = true;
		Sprite->SetSpriteColor(FLinearColor::Green);
	}
}

void ACheckPoint::OverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated)
	{
		const auto Player = Cast<AGBJamCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Player && OtherActor == Player && OtherComp == Player->GetCollider())
		{
			Activate();
			Player->SetRespawnPoint(GetRespawnPoint());
		}
	}
	
}