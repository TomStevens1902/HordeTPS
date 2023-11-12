// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WaveManager.h"
#include "GameFramework/GameModeBase.h"
#include "Player/CharacterBase.h"
#include "UI/HUDBase.h"
#include "Player/PlayerControllerBase.h"
#include "GameFramework/HUD.h"

#include "HordeTPSGameModeBase.generated.h"


/**
 * 
 */
class UHUDInterface;

UCLASS(Blueprintable)
class HORDETPS_API AHordeTPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()



public:
	AHordeTPSGameModeBase();

private:
	void StartPlay() override;


	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	APlayerController* CreateController();
	void SetupControllersHUD(APlayerController* PlayerController);

	FTransform GetPlayerSpawnPoint(AController* Controller);
	ACharacterBase* SpawnCharacter(FTransform SpawnTransform, FActorSpawnParameters SpawnParameters);

	UFUNCTION()
	void RespawnCharacter(APlayerController* PlayerController);


	int ControllerIDCount{ 0 };

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "GameMode | Players")
	TArray<AController*> ExistingControllers;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "GameMode | Players")
	TMap<AController*, ACharacterBase*> CurrentPlayers;

	// Set as a reference by the designer 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GameMode | Wave Manager")
	AWaveManager* WaveManager { nullptr };



	/* Override Character & HUD spawning implementation.
	 * This is done as we want to implement these ourselves. */
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override { return nullptr; }
	virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override {}

};
