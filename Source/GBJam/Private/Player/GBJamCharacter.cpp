// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/GBJamCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "Core/ToolsSubsystem.h"
#include "Projectile/GBJamProjectile.h"
#include "Tools/LadderBlock.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGBJamCharacter

AGBJamCharacter::AGBJamCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(GetRootComponent());
	HitCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HitOverlap);
	HitCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::EndHitOverlap);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	
}

void AGBJamCharacter::Hit()
{
	bIsHitting = true;
	if (Enemy)
	{
		Enemy->ApplyDamage(HitDamage);
	}
}

void AGBJamCharacter::Fire()
{
	if (!ProjectileClass)
	{
		return;
	}
	
	if (bHasLearntFireSkill && bCanFire)
	{
		bCanFire = false;

		const FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		const FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		const auto Projectile = GetWorld()->SpawnActor<AGBJamProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		if (Projectile)
		{
			Projectile->SetDamage(FireDamage);
		}
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::FireCooldown, FireRate);
	}
}

void AGBJamCharacter::Respawn()
{
	HealthComponent->SetMaxHP();
	if (const auto Subsystem = GetWorld()->GetSubsystem<UToolsSubsystem>())
	{
		Subsystem->SetEnemiesMaxHealth();
	}
	SetActorLocation(RespawnPoint);
	GetWorld()->GetFirstPlayerController()->Possess(this);
	bIsAlive = true;
	bIsHitting = false;
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGBJamCharacter::FireCooldown()
{
	bCanFire = true;
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AGBJamCharacter::Climbing()
{

	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + FVector(0.f,-15.f,0.f);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility))
	{
		if (Cast<ALadderBlock>(HitResult.Actor))
		{
			bCanClimb = true;
			auto MC = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (MC)
			{
				MC->Velocity = FVector::ZeroVector;
				MC->bApplyGravityWhileJumping = false;
				MC->GravityScale = 0;
			}
		}
		else
		{
			bCanClimb = false;
			auto MC = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (MC)
			{
				MC->bApplyGravityWhileJumping = true;
				MC->GravityScale = 2;
			}
		}
	}
	else
	{
		bCanClimb = false;
		auto MC = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (MC)
		{
			MC->bApplyGravityWhileJumping = true;
			MC->GravityScale = 2;
		}
	}
}

void AGBJamCharacter::Climb(float Value)
{
	
	if (bCanClimb)
	{
		LaunchCharacter(GetActorUpVector()*Value*UpClimbingSpeed,false,true);
	}
	
	
}

void AGBJamCharacter::StopHitAnim()
{
	bIsHitting = false;
}

void AGBJamCharacter::HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}
	if (const auto EnemyPawn = Cast<IDamageable>(OtherActor))
	{
		Enemy = EnemyPawn;
	}
}
void AGBJamCharacter::EndHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Enemy)
	{
		Enemy = nullptr;
	}
}
//////////////////////////////////////////////////////////////////////////
// Animation

void AGBJamCharacter::UpdateAnimation()
{
	if (!bIsAlive)
	{
		GetSprite()->SetFlipbook(DeathAnimation);
	}

	else if (!bCanFire)
	{
		GetSprite()->SetFlipbook(FireAnimation);
	}
	else if (bIsHitting)
	{
		GetSprite()->SetFlipbook(HitAnimation);
	}

	else if (bCanClimb)
	{
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? ClimbAnimation : ClimbStandAnimation;
		if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
	}
	
	else if (GetMovementComponent()->IsFalling())
	{
		GetSprite()->SetFlipbook(JumpAnimation);
	}
	
	else
	{
		const FVector PlayerVelocity = GetVelocity();
		const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
	}
}

void AGBJamCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();
	Climbing();
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGBJamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &AGBJamCharacter::Hit);
	PlayerInputComponent->BindAction("Hit", IE_Released, this, &AGBJamCharacter::StopHitAnim);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGBJamCharacter::Fire);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGBJamCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Climb", this, &AGBJamCharacter::Climb);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGBJamCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGBJamCharacter::TouchStopped);
}

void AGBJamCharacter::OnDeath()
{
	bIsAlive = false;
	UnPossessed();
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AGBJamCharacter::Respawn, RespawnCooldown);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGBJamCharacter::ApplyDamage(int32 DamageAmount)
{
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
}

void AGBJamCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	if (bCanClimb)
	{
		const FVector NewLocation = GetActorLocation() + FVector(1.f, 0.f, 0.f)*Value*RightClimbingSpeed*GetWorld()->GetDeltaSeconds();
		SetActorLocation(NewLocation);
	}
	else
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
	
}

void AGBJamCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AGBJamCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AGBJamCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

