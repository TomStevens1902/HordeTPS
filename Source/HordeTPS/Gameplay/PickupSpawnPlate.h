// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoPickup.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPlate.generated.h"

UCLASS()
class HORDETPS_API APickupSpawnPlate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupSpawnPlate();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnPickup();

	UFUNCTION()
	void StartPickupCooldown();

	// The time in seconds taken for the spawner to respawn the pickup actor.
	UPROPERTY(EditAnywhere, Category = "Spawner Settings", meta = (AllowPrivateAccess = "true"))
	float SpawnerCooldownTimer{ 30 };

	FTimerHandle TimerHandle_PickupCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Settings", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	AAmmoPickup* CurrentPickup {nullptr};

	UPROPERTY(EditAnywhere, Category = "Spawner Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAmmoPickup> PickupClass;

	UPROPERTY(EditAnywhere, Category = "Spawner Settings", meta = (AllowPrivateAccess = "true"))
	FVector LocalSpawnPositionOffset { 0,0,50 };


};
