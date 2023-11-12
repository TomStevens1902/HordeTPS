// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"




// Sets default values
AWaveManager::AWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	StartWave();
	Super::BeginPlay();
	
}

void AWaveManager::StartWave()
{
	// Determine how many zombies to spawn.
	const float RoundMultiplier = CurrentRound * BaseMultiplier;
	ZombiesToSpawn = MaxAmountOfSpawnedZombies * RoundMultiplier;

	// Reset counts.
	ZombieSpawnedCount = 0;
	PendingSpawnCount = 0;
	DeadZombieCount = 0;

	// Update UI
	OnWaveUpdated.Broadcast(CurrentRound);


	// Grace period.
	GetWorldTimerManager().SetTimer(GracePeriodTimer, this, &AWaveManager::OnGracePeriodEnding, GracePeriodTime, false);
	
}

void AWaveManager::OnGracePeriodEnding()
{
	if (GracePeriodTimer.IsValid()) { GracePeriodTimer.Invalidate(); }
	OnCountdownEnded.Broadcast();
	StartSpawning();
}

void AWaveManager::StartSpawning()
{

	// Spawn as many zombies as possible then stagger spawning
	PendingSpawnCount = (ZombiesToSpawn < MaxAmountOfSpawnedZombies ? ZombiesToSpawn : MaxAmountOfSpawnedZombies);
	TrySpawnEnemy();
}

void AWaveManager::TrySpawnEnemy()
{
	//check for delay
	if(GetWorldTimerManager().GetTimerRemaining(SpawnDelayTimer) < 0.1f)
	{
		SpawnEnemy();
		if (ZombieSpawnedCount < PendingSpawnCount)
		{
			GetWorldTimerManager().SetTimer(SpawnDelayTimer, this, &AWaveManager::TrySpawnEnemy, SpawnDelay, false);
		}
	}
}

FVector AWaveManager::DetermineSpawnPoint()
{
	auto* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	auto const PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	TArray<AWaveSpawnPoint*> TempArray {};
	for(int i = 0; i <= SpawnPoints.Num()-1; i++)
	{
		float SpawnPointXLocation = SpawnPoints[i]->GetActorLocation().X;
		float PlayerXLocation = PlayerLocation.X;
		if (FMath::Abs(PlayerXLocation - SpawnPointXLocation) > InvalidSpawnRegionRadius)
		{
			TempArray.Add(SpawnPoints[i]);
		}
	}
	if(TempArray.IsEmpty())
	{
		return FVector::ZeroVector;
	}
	auto SpawnLocation = TempArray[FMath::RandRange(0, TempArray.Num() - 1)]->FindSpawnPoint();
	return SpawnLocation;
}

void AWaveManager::SpawnEnemy()
{
	// Find appropriate spawn location
	FTransform SpawnTransform{ {0,0,0},{0,0,0},{1,1,1} };
	SpawnTransform.SetLocation(DetermineSpawnPoint());


	// Spawn an AI controller and an Enemy
	FActorSpawnParameters SpawnInfo{};
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	/*FTransform SpawnTransform{ {0,0,0},{0,0,0},{1,1,1} };*/

	AEnemyControllerBase* NewAIController = GetWorld()->SpawnActor<AEnemyControllerBase>(AIControllerClass, SpawnTransform, SpawnInfo);
	AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(EnemyClass, SpawnTransform, SpawnInfo);
	//NewEnemy->OnEnemyDeath.AddDynamic(this, &AWaveManager::OnEnemyDeath);
	NewEnemy->HealthComponent->OnDeath.AddDynamic(this, &AWaveManager::OnEnemyDeath);
	SpawnedEnemies.Add(NewEnemy);

	NewAIController->Possess(NewEnemy);
	if (NewEnemy->Implements<UInitInterface>()) { IInitInterface::Execute_User_Initialisation(NewEnemy); }

	ZombieSpawnedCount++;
}

void AWaveManager::OnEnemyDeath(AActor* KillerActor, AActor* KilledActor)
{
	auto DeadActor = Cast<AEnemyBase>(KilledActor);
	if (SpawnedEnemies.Contains(DeadActor))
	{
		OnDeadEnemy.Broadcast(KillerActor, KilledActor);
		SpawnedEnemies.Remove(DeadActor);
	}

	DeadZombieCount++;
	DetermineWaveComplete();
}

void AWaveManager::DetermineWaveComplete()
{
	if(DeadZombieCount >= ZombiesToSpawn)
	{
		WaveCompleted();
	}
	else if (ZombieSpawnedCount < ZombiesToSpawn && ZombiesToSpawn > MaxAmountOfSpawnedZombies) /* Staggered spawns should only occur when the zombies to spawn 
																								is over the max amount of allowed zombies that are present in
																								the map is achieved. */
	{                          
		// Handles staggering the enemies spawns.
		PendingSpawnCount++;
		TrySpawnEnemy();
	}
}

void AWaveManager::WaveCompleted()
{
	SpawnDelay = FMath::Clamp(SpawnDelay * SpawnDelayMultiplier, 0.08f, 2.0f);

	//increment wave
	CurrentRound++;

	StartWave();
}



// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	if(GracePeriodTimer.IsValid())
	{
		OnCountdownUpdate.Broadcast(GetWorldTimerManager().GetTimerRemaining(GracePeriodTimer));
	}
	Super::Tick(DeltaTime);
}

