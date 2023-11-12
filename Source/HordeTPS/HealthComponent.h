// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, float, CurrentHealth, float, MaxHeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, KillerActor, AActor*, KilledActor);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORDETPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdated OnHealthUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere)
	float MaxHealth{ 100.0f };

	UPROPERTY(EditAnywhere)
	float CurrentHealth{ MaxHealth };

	bool bIsDead {false};

};
