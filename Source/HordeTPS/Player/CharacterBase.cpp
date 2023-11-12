// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Perception/AIPerceptionComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a stimulus source.
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Sight Stimulus"));
	//Setup Stimulus for enemy AI to sense.
	//Sight stimulus is set within the editor. This registers the source with the perception system.
	StimuliSource->RegisterWithPerceptionSystem();

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character
	CameraBoom->TargetOffset.Z = 55.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EquippedSocket = CreateDefaultSubobject<USceneComponent>(TEXT("EquippedWeaponSocket"));
	EquippedSocket->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	UnequippedSocket = CreateDefaultSubobject<USceneComponent>(TEXT("UnequippedWeaponSocket"));
	UnequippedSocket->SetupAttachment(GetMesh(), TEXT("SpineSocket"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;

}

void ACharacterBase::AddToStoredAmmo_Implementation(int InAmmo)
{
	CurrentStoredAmmo = FMath::Clamp(CurrentStoredAmmo + InAmmo, 0, MaxStoredAmmoCapacity);
	OnAmmoUpdated.Broadcast(EquippedWeapon->GetCurrentAmmo(), CurrentStoredAmmo);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

}
void ACharacterBase::User_Initialisation_Implementation()
{
	//const FString ObjectName = "Player " + UGameplayStatics::GetPlayerControllerID(PossessedPlayerController);
	//this->Rename(*ObjectName);
	

	if (GetMovementComponent()) GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	AnimInstance = GetMesh()->GetAnimInstance();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ACharacterBase::OnCharacterDeath);
	}

#pragma region CrouchTimeline
	FOnTimelineFloat CrouchProgressUpdate;
	CrouchProgressUpdate.BindUFunction(this, FName("CrouchUpdate"));

	CrouchingTimeline.AddInterpFloat(CrouchingCurve, CrouchProgressUpdate);

#pragma endregion

#pragma region AimTimeline
	FOnTimelineFloat AimProgressUpdate;
	AimProgressUpdate.BindUFunction(this, FName("AimingUpdate"));

	AimingTimeline.AddInterpFloat(AimingCurve, AimProgressUpdate);


#pragma endregion

	FActorSpawnParameters spawnParams{};
	spawnParams.Instigator = this;
	const FTransform spawnTransform = UKismetMathLibrary::MakeTransform(FVector::ZeroVector, FRotator::ZeroRotator);
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, spawnTransform, spawnParams);
	EquippedWeapon->AttachToComponent(EquippedSocket, FAttachmentTransformRules::KeepRelativeTransform);
	EquippedWeapon->OnShotFired.AddDynamic(this, &ACharacterBase::OnShotFeedback);

	OnAmmoUpdated.Broadcast(EquippedWeapon->GetCurrentAmmo(), CurrentStoredAmmo);
	HealthComponent->OnHealthUpdated.Broadcast(HealthComponent->GetCurrentHealth(), HealthComponent->GetMaxHealth());

	IInitInterface::User_Initialisation_Implementation();
}




// Cleans anything up before the character is destroyed
void ACharacterBase::Destroyed()
{
	// Destroys any equipped weapons.
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	Super::Destroyed();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CrouchingTimeline.TickTimeline(DeltaTime);
	AimingTimeline.TickTimeline(DeltaTime);


	//Checks if the player wants to sprint and can sprint adjusting the movement speed 
	if ((!bIsSprinting) && CanSprint()) {
		ChangeMoveSpeed(SprintingSpeedMultiplier);
		bIsSprinting = true;
	}
	else if (!CanSprint()) {
		ChangeMoveSpeed(bIsAiming ? AimingSpeedMultiplier : 1.0f);
		bIsSprinting = false;
	}

	// Handles trying to shoot for the current weapon
	if (bWantsToShoot && !(bIsSprinting || bIsJumping || bIsReloading))
	{
		if(!bIsShooting)
		{
			bIsShooting = true;
			EquippedWeapon->StartShooting();
		}
	}
	else if (bWantsToShoot && bIsReloading) //add timer clear here & better checking for when reload started
	{
		if(AnimInstance->Montage_GetPosition(ReloadMontage) <= 1.17f)
		{
			AnimInstance->Montage_Stop(0.1f, ReloadMontage);
			bIsReloading = false;
		}
	}
	else
	{
		if(bIsShooting)
		{
			bIsShooting = false;
			EquippedWeapon->StopShooting();
		}
	}
}



void ACharacterBase::Jump()
{
	if (!bIsCrouched) {
		bIsJumping = true;
		Super::Jump();
	}
}

void ACharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCustomMode);
	if (PrevMovementMode == EMovementMode::MOVE_Falling && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling) { bIsJumping = false; }
}

FRotator ACharacterBase::GetAimOffset()
{
	if (IsValid(GetController())) {
		FVector direction = UKismetMathLibrary::Conv_RotatorToVector(GetController()->GetControlRotation());
		FVector inverseDirection = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), direction);
		return (UKismetMathLibrary::Conv_VectorToRotator(inverseDirection));
	}
	return FRotator::ZeroRotator;
}


void ACharacterBase::AimingPressed()
{
	bIsAiming = true;
	ChangeMoveSpeed(AimingSpeedMultiplier);
	AimingTimeline.Play();
}
void ACharacterBase::AimingReleased()
{
	bIsAiming = false;
	ChangeMoveSpeed(1.0f); //1 is used to set the movement speed back to normal 
	AimingTimeline.Reverse();
}

// This is called by the PlayerControllerBase input actions and uses Tick to validate if the player can shoot
void ACharacterBase::ShootingPressed() { bWantsToShoot = true; }
void ACharacterBase::ShootingReleased() { bWantsToShoot = false; }

// This is bound to the weapon's FOnShotFired delegate and called when the weapon shoots.
void ACharacterBase::OnShotFeedback()
{
	// Firing from the hip or while aiming require different montages to be played.
	AnimInstance->Montage_Play(bIsAiming ? AimShootingMontage : HipShootingMontage);
	// This updates the HUD displaying the new ammo count after the weapon has fired.
	OnAmmoUpdated.Broadcast(EquippedWeapon->GetCurrentAmmo(), CurrentStoredAmmo);
}

// This is called by the AimingTimeline
void ACharacterBase::AimingUpdate(float Alpha)
{
	// This zooms the camera giving a sense of better precision.
	FollowCamera->FieldOfView = FMath::Lerp(90.0f, 65.0f, Alpha);
}

void ACharacterBase::CrouchToggle()
{
	if (!bIsCrouched) {
		// Uses built in crouching logic inherited from the character class
		Crouch();
		CrouchingTimeline.Play();
	}
	else
	{
		UnCrouch();
		CrouchingTimeline.Reverse();
	}
}

// This is updated by the CrouchingTimeline  
void ACharacterBase::CrouchUpdate(float Alpha)
{
	CameraBoom->TargetOffset.Z = FMath::Lerp(55.0f, 0.0f, Alpha);
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(90.0f, 60.0f, Alpha));
}


void ACharacterBase::SprintPressed() { bWantsToSprint = true; }
void ACharacterBase::SprintReleased() { bWantsToSprint = false; }

bool ACharacterBase::CanSprint()
{
	FVector Velocity = GetVelocity();
	Velocity.Normalize();

	// validates that the player can sprint
	if (bWantsToSprint 
		&& !bIsCrouched 
		&& !bIsAiming 
		&& !(UKismetMathLibrary::Vector_IsZero(GetVelocity()))
		/// Ensures the player is only able to sprint forward in the direction in which the camera is facing.
		&& (UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Conv_RotatorToVector(GetController()->GetControlRotation()), Velocity) > 0.8f)) { 
		return true;
	}
	return false;
}

/* Modifies the default move speed with a given multiplier.
   This may need to be called with a multiplier of 1 to set the movement mode back to its default. */
void ACharacterBase::ChangeMoveSpeed(float Modifier)
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed * Modifier;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Gets the location of where there player is looking in the world.
FVector ACharacterBase::GetCameraTargetLocation() {
	
	check(GetController()) {
		// Sets up the variables needed for the line trace.
		FHitResult Hit;
		FRotator Rot = GetController()->GetControlRotation();
		FVector Start = FollowCamera->GetComponentLocation();
		FVector End = Start + (Rot.Vector() * ViewTraceDistance);
		FCollisionQueryParams TraceParams;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		// If the player is looking somewhere to far away then the end location of the trace is used instead.
		FVector TargetLocation = (Hit.bBlockingHit ? (Hit.Location + (Hit.ImpactNormal * -2.0f)) : End);
		return TargetLocation;
	}
}

// This is an interfaced function called from an anim notify on the reload montage.
void ACharacterBase::ReloadNotifyPoint_Implementation()
{
	// Ammo is added at a specific point in the animation
	int AmmoNeeded = EquippedWeapon->GetMaxAmmo() - EquippedWeapon->GetCurrentAmmo();
	if (CurrentStoredAmmo - AmmoNeeded < 0)
	{
		EquippedWeapon->ReloadAmmo(CurrentStoredAmmo);
		CurrentStoredAmmo = 0;
	}
	else
	{
		EquippedWeapon->ReloadAmmo(AmmoNeeded);
		CurrentStoredAmmo -= AmmoNeeded;
	}

	OnAmmoUpdated.Broadcast(EquippedWeapon->GetCurrentAmmo(), CurrentStoredAmmo);;
}


// Plays the reload animation triggering the reload functionality through the animation.
void ACharacterBase::ReloadStart()
{
	if (!bIsReloading && CurrentStoredAmmo > 0 && EquippedWeapon->CanReload())
	{
		bIsReloading = true;
		AnimInstance->Montage_Play(ReloadMontage);
		GetWorldTimerManager().SetTimer(HandleReloadingTimer, this, &ACharacterBase::OnReloadMontageEnd, ReloadMontage->GetPlayLength(), false);
	}
}

// This signifies when the animation ends allowing the player to shoot again.
void ACharacterBase::OnReloadMontageEnd()
{
	bIsReloading = false;
}



// Called by the health component FOnDeath delegate when health reaches 0.
void ACharacterBase::OnCharacterDeath(AActor* KillerActor, AActor* KilledActor)
{
#pragma once

	if (StimuliSource)
	{
		//StimuliSource->UnregisterFromSense(TSubclassOf<UAISense_Sight>());
		StimuliSource->UnregisterFromPerceptionSystem();
	}
	  
	// Prevent any functionality to happen on tick and disable the players input.
	SetActorTickEnabled(false);
	GetController()->DisableInput(PossessedPlayerController);


	// Stop shooting and prevent shooting from persisting. The weapon is also detached and needs destroying.
	bIsShooting = false;
	if(EquippedWeapon)
	{
		EquippedWeapon->StopShooting();
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->GetMesh()->SetSimulatePhysics(true);
		EquippedWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}


	// Causes the player to rag doll and prevents any collision interactions 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->bPauseAnims = true;

	// Sets a timer to wait for before respawning the character using the game mode.
	GetWorldTimerManager().SetTimer(HandleRespawnTime, this, &ACharacterBase::RespawnCharacter, RespawnTime, false);

	OnCharacterDied.Broadcast(this);
}

void ACharacterBase::RespawnCharacter()
{
#pragma once
	// On respawn bound in AHordeTPSGameModeBase keeps player persistent 
	OnRespawn.Broadcast(PossessedPlayerController);
}

void ACharacterBase::SetupStimulusSource()
{

}