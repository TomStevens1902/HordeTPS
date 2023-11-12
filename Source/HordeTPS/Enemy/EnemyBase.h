// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "HordeTPS/HealthComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "HordeTPS/ActorInfoInterface.h"
#include "HordeTPS/Consts.h"
#include "HordeTPS/InitInterface.h"
#include "Kismet/GameplayStatics.h"

#include "EnemyBase.generated.h"


UCLASS()
class HORDETPS_API AEnemyBase : public ACharacter, public ICombatInterface, public IInitInterface, public IActorInfoInterface
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	class UBoxComponent* RightHandCollisionBox;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = true))
	//class UBoxComponent* LeftHandCollisionBox;

public:
	// Sets default values for this pawn's properties
	AEnemyBase();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FText GetActorDisplayName_Implementation() override { return DisplayName; }

	UBehaviorTree* GetBehaviourTree() const { return BehaviorTree; }

	UAnimMontage* GetMontage() const { return Montage; }

	virtual void MeleeAttack_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

private:

	UPROPERTY(BlueprintReadWrite, Category = "Actor Info", meta = (AllowPrivateAccess = "true"))
	FText DisplayName{ FText::FromString("Zombie") };


	UFUNCTION()
	void OnDeath(AActor* KillerActor, AActor* KilledActor);
	UFUNCTION()
	void OnRagdollLifetimeEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	UBehaviorTree* BehaviorTree{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking", meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> AttackingMontages;

	UPROPERTY()
	UAnimMontage* Montage{ nullptr };


	// Overlap
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking", meta = (AllowPrivateAccess = true))
	float MeleeDamage{ 50 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking", meta = (AllowPrivateAccess = true))
	bool CanAttack{ true };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking", meta = (AllowPrivateAccess = true))
	float AttackCooldownTime{ 1.5f };

	void AttackCooldownStart(float CooldownTime);
	void AttackCooldownEnd();
};
