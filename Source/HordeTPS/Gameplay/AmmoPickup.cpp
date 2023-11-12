// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickup.h"
#include "HordeTPS/Player/PlayerInterface.h"
#include "HordeTPS/Player/CharacterBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAmmoPickup::AAmmoPickup() :
	CollisionSphere {CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere")) },
	MeshComponent{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))},
	MovementComponent{CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"))}
{
	if(MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
	}
	if(CollisionSphere)
	{
		CollisionSphere->SetupAttachment(MeshComponent);
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::BeginOverlap);
	}


}

void AAmmoPickup::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()) {
		if (OtherActor->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToStoredAmmo(OtherActor, AmmoToAdd);
			OnAmmoPickup.Broadcast();

			Destroy();
		}
	}
}
