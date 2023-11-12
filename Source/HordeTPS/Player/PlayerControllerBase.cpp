// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::OnPossess(APawn* aPawn)
{
	// Call the parent method, to let it do anything it needs to
	Super::OnPossess(aPawn);

	// Store a reference to the Player's Pawn
	PlayerCharacter = Cast<ACharacterBase>(aPawn);
	checkf(PlayerCharacter, TEXT("APlayerControllerBBBase derived classes should only posess ACharacterBBBase derived pawns"));
	PlayerCharacter->PossessedPlayerController = this;

	// Get a reference to the EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get reference to the EnhancedInputComponent."));

	// Get the local player subsystem
	// Just a local variable, we don't need to refer to it again after this
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem."));

	// Wipe existing mappings, and add our mapping.
	checkf(InputMappingContent, TEXT("InputMappingContent was not specified."));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContent, 0);

	// Bind the input actions.
	// Only attempt to bind if valid values were provided.
	if (ActionMove) {
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleMove);
	}

	if (ActionLook) {
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleLook);
	}

	if (ActionJump) {
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleJump);
	}

	if (ActionCrouch){
		EnhancedInputComponent->BindAction(ActionCrouch, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleCrouch);
	}

	if (ActionSprint) {
		EnhancedInputComponent->BindAction(ActionSprint, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleSprint);
	}

	if (ActionAim) {
		EnhancedInputComponent->BindAction(ActionAim, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleAim);
	}

	if (ActionShoot) {
		EnhancedInputComponent->BindAction(ActionShoot, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleShoot);
	}

	if(ActionReload)
	{
		EnhancedInputComponent->BindAction(ActionReload, ETriggerEvent::Triggered, this, &APlayerControllerBase::HandleReload);
	}

}

void APlayerControllerBase::OnUnPossess()
{
	// Unbind things here...
	EnhancedInputComponent->ClearActionBindings();
	// Dereference the controller.
	PlayerCharacter->PossessedPlayerController = nullptr;
	// Call the parent method, in case it needs to do anything.
	Super::OnUnPossess();
}


void APlayerControllerBase::HandleMove(const FInputActionValue& InputActionValue)
{
	// Value is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Add movement to the Player's Character Pawn
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
	}
}


void APlayerControllerBase::HandleLook(const FInputActionValue& InputActionValue)
{
	// Input is a Vector2D
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// Add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);

	AddPitchInput(LookAxisVector.Y);
}

void APlayerControllerBase::HandleJump()
{
	// Input is 'Digital' (value not used here)

	// Make the Player's Character Pawn jump, disabling crouch if it was active
	if (PlayerCharacter)
	{
		PlayerCharacter->Jump();
	}
}

void APlayerControllerBase::HandleCrouch()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->CrouchToggle();
	}
}

void APlayerControllerBase::HandleAim(const FInputActionValue& InputActionValue)
{
	if (PlayerCharacter)
	{
		if(InputActionValue.Get<bool>())
		{
			PlayerCharacter->AimingPressed();
		}
		else
		{
			PlayerCharacter->AimingReleased();
		}
	}
}

void APlayerControllerBase::HandleSprint(const FInputActionValue& InputActionValue)
{
	if (PlayerCharacter)
	{
		if (InputActionValue.Get<bool>())
		{
			PlayerCharacter->SprintPressed();
		}
		else
		{
			PlayerCharacter->SprintReleased();
		}
	}
}

void APlayerControllerBase::HandleShoot(const FInputActionValue& InputActionValue)
{
	if (PlayerCharacter)
	{
		if (InputActionValue.Get<bool>())
		{
			PlayerCharacter->ShootingPressed();
		}
		else
		{
			PlayerCharacter->ShootingReleased();
		}
	}
}

void APlayerControllerBase::HandleReload()
{
	if (PlayerCharacter) { PlayerCharacter->ReloadStart(); }
}
