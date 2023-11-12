// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"



// Sets default values
AEnemyBase::AEnemyBase() :
	RightHandCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")) }
	//LeftHandCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")) }
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	if (HealthComponent)
	{
		
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyBase::OnDeath);
	}

	if(RightHandCollisionBox)
	{
		RightHandCollisionBox->SetupAttachment(GetMesh(), EnemyConsts::RightHandSocketName);
		RightHandCollisionBox->SetRelativeLocation(EnemyConsts::CollisionBoxLocation);
		RightHandCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::BeginOverlap);
	}

	//if (LeftHandCollisionBox)
	//{
	//	LeftHandCollisionBox->SetupAttachment(GetMesh(), EnemyConsts::LeftHandSocketName);
	//	LeftHandCollisionBox->SetRelativeLocation(EnemyConsts::CollisionBoxLocation);
	//	LeftHandCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::BeginOverlap);
	//}
	
}




// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{ 
	Super::Tick(DeltaTime);
}


void AEnemyBase::MeleeAttack_Implementation()
{
	Montage = AttackingMontages[FMath::RandRange(0, AttackingMontages.Num() - 1)];
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void AEnemyBase::OnDeath(AActor* KillerActor, AActor* KilledActor)
{
	// Causes the player to rag doll and prevents any collision interactions
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->bPauseAnims = true;

	// Prevent the enemy from causing any damage after its death
	RightHandCollisionBox->SetGenerateOverlapEvents(false);

	// Sets a timer to wait for before respawning the character using the game mode.
	FTimerHandle RagdollLifetimeTimer;
	GetWorldTimerManager().SetTimer(RagdollLifetimeTimer, this, &AEnemyBase::OnRagdollLifetimeEnd, 5.f, false);
}

void AEnemyBase::OnRagdollLifetimeEnd()
{
	Destroy();
}

void AEnemyBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Applies damage to a player once before going on cooldown.
	if (CanAttack && Montage)
	{
		// Get the controller pawn.
		auto const* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		// Ensures we are only attacking the player.
		if (OtherActor == Player)
		{
			// Damage is received by the player through the UGameplayStatics receiveAnyDamage function in its health component.
			UGameplayStatics::ApplyDamage(OtherActor, MeleeDamage, GetController(), this, TSubclassOf<UDamageType>());

			// Sets the attack cooldown time to the remaining time of the montage. 
			AttackCooldownStart(Montage->GetPlayLength() - GetMesh()->GetAnimInstance()->Montage_GetPosition(Montage));
		}
	}
}

void AEnemyBase::AttackCooldownStart(float CooldownTime)
{
	// This prevents the enemy from being able to preform multiple attacks per hit
	CanAttack = false;
	FTimerHandle AttackCooldownTimer;
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &AEnemyBase::AttackCooldownEnd, CooldownTime, false);
}

void AEnemyBase::AttackCooldownEnd()
{
	CanAttack = true;
}



