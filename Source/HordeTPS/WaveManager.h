// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveSpawnPoint.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyControllerBase.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#include "WaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveUpdated, int, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdate, float, CountdownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountdownEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRegisterDeadEnemy, AActor*, KillerActor, AActor*, KilledActor);

UCLASS(Blueprintable)
class HORDETPS_API AWaveManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaveManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	FOnWaveUpdated OnWaveUpdated;
	FOnCountdownUpdate OnCountdownUpdate;
	FOnCountdownEnded OnCountdownEnded;
	FRegisterDeadEnemy OnDeadEnemy;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	// Zombie amount = (Round number * BaseMultiplier) * MaxAmountOfSpawnedZombies




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	int ZombiesToSpawn{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	int CurrentRound { 1 }; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	int MaxAmountOfSpawnedZombies { 24 }; //24 for 1 then +6 for any extra players

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	float BaseMultiplier{ 0.15f };

	// The delay time in seconds. It is initially set to its starting delay time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	float SpawnDelay{ 2.0f };

	// The multiplier applied to the current spawn delay each round.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	float SpawnDelayMultiplier{ 0.95f };

	// The time between the wave starting and spawning occurring.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	float GracePeriodTime{ 8.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	TSubclassOf<AEnemyControllerBase> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	TArray<AWaveSpawnPoint*> SpawnPoints;


	// The radius around a player that prevents spawn points from working. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	float InvalidSpawnRegionRadius{ 1000 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	TArray<AEnemyBase*> SpawnedEnemies;

	FTimerHandle GracePeriodTimer;
	FTimerHandle SpawnDelayTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	int DeadZombieCount{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	int ZombieSpawnedCount{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner Values", meta = (AllowPrivateAccess = true))
	int PendingSpawnCount{ 0 };

	void StartWave();
	void OnGracePeriodEnding();
	void StartSpawning();
	void TrySpawnEnemy();
	FVector DetermineSpawnPoint();
	void SpawnEnemy();
	void DetermineWaveComplete();
	void WaveCompleted();
	UFUNCTION()
	void OnEnemyDeath(AActor* KillerActor, AActor* KilledActor);

	//TESTING
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings", meta = (AllowPrivateAccess = true))
	//AWaveSpawnPoint* SpawnPoint{ nullptr };
};
