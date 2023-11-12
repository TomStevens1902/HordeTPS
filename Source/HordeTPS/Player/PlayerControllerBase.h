// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "CharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

// Forward Declarations
class ACharacterBase;
class UInputMappingContext;
class UInputAction;

UCLASS(Abstract)
class HORDETPS_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	// The Input Action to map to movement.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* ActionMove = nullptr;

	// The Input Action to map to looking around.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* ActionLook = nullptr;

	// The Input Action to map to jumping.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* ActionJump = nullptr;

	// The Input Action to map to sprinting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* ActionSprint = nullptr;

	// The Input Action to map to crouching
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	UInputAction* ActionCrouch = nullptr;

	// The Input Action to map to aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Combat")
	UInputAction* ActionAim = nullptr;

	// The Input Action to map to shoot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Combat")
	UInputAction* ActionShoot = nullptr;

	// The Input Action to map to reload
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Combat")
	UInputAction* ActionReload = nullptr;

	// The Input Mapping Context to use.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Settings")
	UInputMappingContext* InputMappingContent = nullptr;

protected:
	// Action Handler Functions
	void         HandleLook(const FInputActionValue& InputActionValue);
	void         HandleMove(const FInputActionValue& InputActionValue);
	void         HandleJump();
	void		 HandleCrouch();
	void		 HandleAim(const FInputActionValue& InputActionValue);
	void		 HandleSprint(const FInputActionValue& InputActionValue);
	void		 HandleShoot(const FInputActionValue& InputActionValue);
	void		 HandleReload(); 

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

private:
	// Used to store a reference to the InputComponent cast to an EnhancedInputComponent.
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	// Used to store a reference to the pawn we are controlling.
	UPROPERTY()
	ACharacterBase* PlayerCharacter = nullptr;

};
