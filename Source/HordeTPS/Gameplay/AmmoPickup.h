// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HordeTPS/Player/CharacterBase.h"
#include "GameFramework/RotatingMovementComponent.h"


#include "AmmoPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickup);

UCLASS()
class HORDETPS_API AAmmoPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmoPickup();

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnPickup OnAmmoPickup;

protected:

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Settings", meta = (AllowPrivateAccess = true))
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Settings", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Settings", meta = (AllowPrivateAccess = true))
	class URotatingMovementComponent* MovementComponent;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, Category = "Pickup Settings", meta = (AllowPrivateAccess = "true"))
	int AmmoToAdd {120};
};
