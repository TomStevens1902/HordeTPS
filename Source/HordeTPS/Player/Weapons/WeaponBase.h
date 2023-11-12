// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileBase.h"


#include "WeaponBase.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShotFired);

UCLASS()
class HORDETPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;



public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void StartShooting();

	UFUNCTION()
	void StopShooting();

	UFUNCTION()
	void ReloadAmmo(int AmmoIn);

	UPROPERTY(BlueprintAssignable, Category = "delegate")
	FOnShotFired OnShotFired;

	UFUNCTION()
	int GetMaxAmmo() { return MaxAmmo; }

	UFUNCTION()
	int GetCurrentAmmo() { return CurrentAmmo; }

	UFUNCTION()
	bool CanReload();

	UFUNCTION()
	USkeletalMeshComponent* GetMesh() { return Mesh; }

private:
	// The amount of ammo the weapon can carry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Weapon Settings")
	int MaxAmmo {30};

	// The current amount of ammo the weapon has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Weapon Settings")
	int CurrentAmmo {MaxAmmo};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Weapon Settings")
	TSubclassOf<AProjectileBase> Projectile;

	UFUNCTION()
	void SpawnProjectile();

	// The seconds to wait between shots
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Weapon Settings")
	float TimeBetweenShots {0.1f};

	void OnCooldownEnded();

	bool TryingToShoot {false};
	bool OnCooldown{ false };
	FTimerHandle HandleTimeBetweenShots;
	FTimerHandle HandleShootingCooldown; // Used to prevent the timer being reset every input

};