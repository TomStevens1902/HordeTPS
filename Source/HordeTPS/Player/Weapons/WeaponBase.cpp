// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	//ShootingFeedback::Execute();

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Shoot()
{
	--CurrentAmmo;
	SpawnProjectile();
	OnShotFired.Broadcast();
	if (CurrentAmmo <= 0)
	{
		StopShooting();
	}
}

void AWeaponBase::StartShooting()
{
	TryingToShoot = true;
	if(!OnCooldown && CurrentAmmo > 0)
	{
		Shoot();
		GetWorldTimerManager().SetTimer(HandleTimeBetweenShots, this, &AWeaponBase::Shoot, TimeBetweenShots, true);
	}
}


void AWeaponBase::StopShooting()
{
	TryingToShoot = false;
	// Check to see if shooting ended prematurely
	if(GetWorldTimerManager().GetTimerRemaining(HandleTimeBetweenShots) > 0.0f && !OnCooldown)
	{
		OnCooldown = true;
		float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(HandleTimeBetweenShots);
		GetWorldTimerManager().SetTimer(HandleShootingCooldown, this, &AWeaponBase::OnCooldownEnded, TimeRemaining, false);
	}
	GetWorldTimerManager().ClearTimer(HandleTimeBetweenShots);
}

// This is used to refill the current ammo of the weapon. 
void AWeaponBase::ReloadAmmo(int AmmoIn)
{
	CurrentAmmo = UKismetMathLibrary::Clamp(CurrentAmmo += AmmoIn, 0, MaxAmmo);
}

bool AWeaponBase::CanReload()
{
	bool CanReload = CurrentAmmo != MaxAmmo;
	return CanReload;
}


void AWeaponBase::OnCooldownEnded()
{
	OnCooldown = false;
	TryingToShoot? 
		StartShooting() :
		GetWorldTimerManager().ClearTimer(HandleShootingCooldown) ;
}


void AWeaponBase::SpawnProjectile()
{
	FVector TargetLocation{};
	IPlayerInterface* Interface = Cast<IPlayerInterface>(GetInstigator());
	if (Interface) { TargetLocation = Interface->GetCameraTargetLocation(); }

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);
	GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation, SpawnParameters);
}
