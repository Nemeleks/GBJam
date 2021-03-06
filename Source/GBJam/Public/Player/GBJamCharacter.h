// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Interfaces/Damageable.h"
#include "GBJamCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for GBJam, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AGBJamCharacter : public APaperCharacter, public IDamageable
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;
	
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HitAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ClimbAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ClimbStandAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float RespawnCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 HitDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	int32 FireDamage = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class AGBJamProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Climbing")
	float UpClimbingSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Climbing")
	float RightClimbingSpeed = 250.f;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void StopHitAnim();

	UFUNCTION()
	void HitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void EndHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	AGBJamCharacter();

	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetCollider() const {return Cast<UPrimitiveComponent>(GetRootComponent());}
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(int32 DamageAmount) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsAlive() const {return bIsAlive;}

	UFUNCTION(BlueprintCallable)
	void EnableDoubleJump() {JumpMaxCount = 2;}

	UFUNCTION(BlueprintCallable)
	void EnableFiring(){bHasLearntFireSkill = true;}

	void SetRespawnPoint(FVector NewPoint) {RespawnPoint = NewPoint;}

private:
	void Hit();
	void Fire();
	void Respawn();
	void FireCooldown();
	void Climbing();
	void Climb(float Value);
	
	FVector RespawnPoint;
	bool bIsHitting = false;
	bool bIsAlive = true;
	bool bCanFire = true;
	bool bHasLearntFireSkill = false;
	bool bIsClimbing = false;
	bool bCanClimb = false;
	FTimerHandle RespawnTimerHandle;
	FTimerHandle FireTimerHandle;

	IDamageable* Enemy = nullptr;
};

inline void AGBJamCharacter::BeginPlay()
{
	Super::BeginPlay();
	RespawnPoint = GetWorld()->GetFirstPlayerController()->GetSpawnLocation();
}



