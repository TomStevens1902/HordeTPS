// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSpawnPlate.h"

#include "AmmoPickup.h"

// Sets default values
APickupSpawnPlate::APickupSpawnPlate() :
	MeshComponent{ CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")) }
{
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}
}

void APickupSpawnPlate::BeginPlay()
{
	SpawnPickup();
}

void APickupSpawnPlate::SpawnPickup()
{
	if(CurrentPickup != nullptr)
	{
		CurrentPickup->Destroy();
		CurrentPickup = nullptr;
	}

	if (TimerHandle_PickupCooldown.IsValid()) { TimerHandle_PickupCooldown.Invalidate(); }


	FActorSpawnParameters spawnParams{};
	FTransform SpawnLocation = {
		{0,0,0},
		GetActorLocation()+LocalSpawnPositionOffset,
		{1,1,1} };
	CurrentPickup = GetWorld()->SpawnActor<AAmmoPickup>(PickupClass, SpawnLocation , spawnParams);

	CurrentPickup->OnAmmoPickup.AddDynamic(this, &APickupSpawnPlate::StartPickupCooldown);
}

void APickupSpawnPlate::StartPickupCooldown()
{
	GetWorldTimerManager().SetTimer(TimerHandle_PickupCooldown, this, &APickupSpawnPlate::SpawnPickup, SpawnerCooldownTimer, false);
}
