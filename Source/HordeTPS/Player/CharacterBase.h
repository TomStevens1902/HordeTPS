// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "HordeTPS/ActorInfoInterface.h"
#include "HordeTPS/HealthComponent.h"
#include "HordeTPS/InitInterface.h"
#include "Weapons/WeaponBase.h"



#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoUpdated, int, CurrentAmmo, int, StoredAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRespawn,APlayerController*,PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, AActor*, ActorReference);

// Forward declaration's
class UWeaponBase;
class UCameraComponent;
class UCurveFloat;
class UHealthComponent;

UCLASS(Blueprintable)
class HORDETPS_API ACharacterBase : public ACharacter, public IPlayerInterface , public IInitInterface, public IActorInfoInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UFUNCTION()
	void AddToStoredAmmo_Implementation(int InAmmo) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void User_Initialisation_Implementation() override;

	virtual FText GetActorDisplayName_Implementation() override { return DisplayName; }

	//UFUNCTION()
	//UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	// Reference to the currently possessing player controller. This is set by the controller on possess and unpossess. 
	UPROPERTY()
	APlayerController* PossessedPlayerController{ nullptr };

	// Called on reload and shooting. Used in AHUDBase to display current ammo
	UPROPERTY(BlueprintAssignable)
	FOnAmmoUpdated OnAmmoUpdated;

	// Called after death, telling the game mode to respawn the character.
	UPROPERTY(BlueprintAssignable)
	FOnRespawn OnRespawn;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDied OnCharacterDied;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	// Player Interface functions
	UFUNCTION()
	virtual FVector GetCameraTargetLocation() override;

	UFUNCTION()
	virtual void ReloadNotifyPoint_Implementation() override;


	// These Functions are called by the controller which handles the players inputs. 
	void CrouchToggle();

	void SprintPressed();
	void SprintReleased();

	void AimingPressed();
	void AimingReleased();

	void ShootingPressed();
	void ShootingReleased();

	void ReloadStart();

	virtual void Jump() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, Category = "Character Info", meta = (AllowPrivateAccess = "true"))
	FText DisplayName {FText::FromString("Player")};

	//Character State used for validating actions and animating the player
	UPROPERTY(BlueprintReadOnly, Category = "Character State", meta = (AllowPrivateAccess = "true"))
	bool bIsJumping{ false };

	UPROPERTY(BlueprintReadOnly, Category = "Character State", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming{ false };

	UPROPERTY(BlueprintReadOnly, Category = "Character State", meta = (AllowPrivateAccess = "true"))
	bool bIsReloading{ false };

	UPROPERTY(BlueprintReadOnly, Category = "Character State", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting{ false };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetAimOffset();


	UPROPERTY()
	UAnimInstance* AnimInstance{ nullptr };


#pragma region Components
	// Spring arm for camera connection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* EquippedSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* UnequippedSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;
	TSubclassOf<UAISense_Sight> SightClass;
	void SetupStimulusSource();


#pragma endregion


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	TSubclassOf<AWeaponBase> WeaponToSpawn;

	UPROPERTY()
	AWeaponBase* EquippedWeapon{ nullptr };


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	int MaxStoredAmmoCapacity{ 300 };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	int CurrentStoredAmmo{ MaxStoredAmmoCapacity };

	// Shooting animations set in the character blueprint.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	UAnimMontage* HipShootingMontage{ nullptr };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	UAnimMontage* AimShootingMontage{ nullptr };
	// Bound to the weapon and called when shot is fired.
	UFUNCTION()
	void OnShotFeedback();


	// Reloading animation set in the character blueprint.
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Character Settings")
	UAnimMontage* ReloadMontage{ nullptr };

	// Timer responsible for timing the end of the reload montage.
	FTimerHandle HandleReloadingTimer;

	// Called by the timer signifying the end of the montage.
	void OnReloadMontageEnd();


	/* References Curve assets used in their respected timeline's,
	   these are set in the characters blueprint */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	UCurveFloat* CrouchingCurve{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	UCurveFloat* AimingCurve{ nullptr };

	// Handles the transition between default camera position to aiming.
	FTimeline AimingTimeline;

	// Called by the timeline used to update the players FOV.
	UFUNCTION()
	void AimingUpdate(float Alpha);

	// Handles he transition between standing and crouching.
	FTimeline CrouchingTimeline;

	// Called by the timeline used to update the cameras position and the collision components half height.
	UFUNCTION()
	void CrouchUpdate(float Alpha);


	// Shooting related variables
	bool bWantsToShoot{ false };
	bool bIsShooting{ false };

	// Sprint related variables
	bool bWantsToSprint{ false };
	bool CanSprint();

	// Used to tell when the player has hit the ground and no longer jumping.
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;


	// Base move speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	float DefaultMovementSpeed{ 400 };
	// Aiming move speed multiplier, 1 = default movement speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	float AimingSpeedMultiplier{ 0.5f };
	// Sprinting speed multiplier, 1 = default movement speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	float SprintingSpeedMultiplier{ 2.5f };
	 
	// The max trace distance used to determine the location the player is looking at.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	float ViewTraceDistance{ 2500 };


	// Changes the movement speed by modifying the default movement speed with a multiplier.
	// e.g (1 = default move speed, 2 = twice the move speed, 0.5 = half the move speed).
	void ChangeMoveSpeed(float Modifier);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FTimerHandle HandlePlayerDeathTime;
	// Plays once and is responsible for preforming the characters death functionality
	UFUNCTION()
	void OnCharacterDeath(AActor* KillerActor, AActor* KilledActor);

	FTimerHandle HandleRespawnTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Settings", meta = (AllowPrivateAccess = true))
	float RespawnTime{ 3.0f };
	UFUNCTION()
	void RespawnCharacter();
};
