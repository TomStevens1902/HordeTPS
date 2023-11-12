// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "HordeTPS/HealthComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//component setup
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::BeginOverlap);
	Sphere->OnComponentHit.AddDynamic(this, &AProjectileBase::Hit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ProjectileLifetime, this, &AProjectileBase::ProjectileLifetimeEnd, 5.0f, false, -1.0f);
}

void AProjectileBase::ProjectileLifetimeEnd()
{
	GetWorldTimerManager().ClearTimer(ProjectileLifetime);
	DestroyProjectile();

}

void AProjectileBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DestroyProjectile();
}

void AProjectileBase::Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigator()->GetController(), GetOwner(), TSubclassOf<UDamageType>());

	if(OnHitParticle)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnHitParticle, Hit.Location, FRotator::ZeroRotator,FVector(1),true);

	}
	DestroyProjectile();
}


// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectileBase::DestroyProjectile()
{
	this->Destroy();
}