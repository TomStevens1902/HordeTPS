// Copyright Epic Games, Inc. All Rights Reserved.


#include "HordeTPSGameModeBase.h"

#include "UI/HUDBase.h"


AHordeTPSGameModeBase::AHordeTPSGameModeBase()
{

}

void AHordeTPSGameModeBase::StartPlay()
{

	// Before Begging play any existing controllers need to be removed.
	for (int i = 0; i <= ExistingControllers.Num(); i++)
	{
		// This ensures that there no controllers currently populating and controller ID's.
		UGameplayStatics::RemovePlayer(Cast<APlayerController>(ExistingControllers[i]), true);
	}
	// Once any existing players are cleaned up we can ensure we start with an empty list.
	auto NewController = CreateController();


	FActorSpawnParameters SpawnInfo{};
	auto NewCharacter = SpawnCharacter(GetPlayerSpawnPoint(NewController),SpawnInfo);

	if (NewCharacter && NewController)
	{
		NewController->Possess(NewCharacter);
		CurrentPlayers.Add(NewController, NewCharacter);
	}

	//Create wave manager
	WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));

	//Bind wave manager stuff
	SetupControllersHUD(NewController);

	//Initialise chain here
	if (auto const* IInit = Cast<IInitInterface>(NewCharacter))
	{
		IInit->Execute_User_Initialisation(NewCharacter);
	}

	Super::StartPlay();
}

APlayerController* AHordeTPSGameModeBase::CreateController()
{
	// Attempt to create a new local player. This spawns a controller of the default type set in the game mode blueprint.
	APlayerController* NewController = UGameplayStatics::CreatePlayer(GetWorld(), ControllerIDCount, true);
	// If successful then try create the HUD & Character for the controller.
	if (NewController != nullptr)
	{
		// Each time a player is created a new ID is needed
		ControllerIDCount++;
		return NewController;
	}
	return nullptr;
}

void AHordeTPSGameModeBase::SetupControllersHUD(APlayerController* PlayerController)
{
	PlayerController->ClientSetHUD(HUDClass);
	AHUDBase* PlayerHUD = Cast<AHUDBase>(PlayerController->GetHUD());
	PlayerHUD->ClearHUD();
	PlayerHUD->CreateHUD();

	auto Character = *CurrentPlayers.Find(PlayerController);
	//Bind stuff here: Player display, Wave management, Kill Feeding
	Character->OnAmmoUpdated.AddDynamic(PlayerHUD, &AHUDBase::UpdateAmmo);
	Character->HealthComponent->OnHealthUpdated.AddDynamic(PlayerHUD, &AHUDBase::UpdateHealth);
	Character->HealthComponent->OnDeath.AddDynamic(PlayerHUD, &AHUDBase::DisplayDeath);

	WaveManager->OnWaveUpdated.AddDynamic(PlayerHUD, &AHUDBase::UpdateWave);
	WaveManager->OnCountdownUpdate.AddDynamic(PlayerHUD, &AHUDBase::UpdateWaveCountdown);
	WaveManager->OnCountdownEnded.AddDynamic(PlayerHUD, &AHUDBase::EndCountdown);
	WaveManager->OnDeadEnemy.AddDynamic(PlayerHUD, &AHUDBase::DisplayDeath);
}


FTransform AHordeTPSGameModeBase::GetPlayerSpawnPoint(AController* Controller)
{
	return ChoosePlayerStart(Controller)->GetActorTransform();
}


ACharacterBase* AHordeTPSGameModeBase::SpawnCharacter(FTransform SpawnTransform, FActorSpawnParameters SpawnParameters)
{
	ACharacterBase* NewCharacter = GetWorld()->SpawnActor<ACharacterBase>(DefaultPawnClass, SpawnTransform, SpawnParameters);
	if(NewCharacter)
	{
		
		NewCharacter->OnRespawn.AddDynamic(this, &AHordeTPSGameModeBase::RespawnCharacter);
		return NewCharacter;
	}
	return nullptr;
}


void AHordeTPSGameModeBase::RespawnCharacter(APlayerController* PlayerController)
{
	if(PlayerController->GetPawn())
	{
		// Destroy the previous dead player character.
		PlayerController->GetPawn()->Destroy();
		PlayerController->UnPossess();
	}

	FActorSpawnParameters SpawnInfo{};
	auto NewCharacter = SpawnCharacter(GetPlayerSpawnPoint(PlayerController), SpawnInfo);

	if (NewCharacter)
	{
		PlayerController->Possess(NewCharacter);
		// Amend list of connected characters to controllers.
		CurrentPlayers.Add(PlayerController, NewCharacter);
	}

	SetupControllersHUD(PlayerController);
	if (NewCharacter->Implements<UInitInterface>()) { IInitInterface::Execute_User_Initialisation(NewCharacter); }

	PlayerController->EnableInput(PlayerController);
}



// This keeps track of any currently existing controllers.
void AHordeTPSGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ExistingControllers.Add(NewPlayer);
}


// This corrects the list of controllers when any are removed.
void AHordeTPSGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ExistingControllers.Remove(Exiting);
}
