// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "WaveSpawnPoint.generated.h"

UCLASS(Blueprintable)
class HORDETPS_API AWaveSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaveSpawnPoint();

	UFUNCTION()
	FVector FindSpawnPoint();

	FVector GetRandomPointInBox();
protected:

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnBox;

	

	//TrySpawn return bool
	//ValidateSpawn return bool (trace here for overlap)
	//SpawnEnemy (consider spawn successful if this happens)
};
