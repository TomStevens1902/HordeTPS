// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KillFeed.h"
#include "WaveInfo.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "HordeTPS/Player/CharacterBase.h"

#include "WeaponDisplay.h"

#include "HUDBase.generated.h"

/**
 * 
 */
class ACharacterBase;

UCLASS()
class HORDETPS_API AHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	AHUDBase();

	UFUNCTION()
	void UpdateAmmo(int CurrentAmmo, int StoredAmmo);

	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void UpdateWave(int WaveNumber) { WaveInfo->SetWaveNumber(WaveNumber); }

	UFUNCTION()
	void UpdateWaveCountdown(float InTime) { WaveInfo->SetCountdownTime(InTime); }
	UFUNCTION()
	void EndCountdown() { WaveInfo->OnCountdownEnded(); }
	UFUNCTION()
	void DisplayDeath(AActor* KillerActor, AActor* DeadActor);

	void ClearHUD();
	void CreateHUD();

private:

	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WeaponDisplayClass;
	UPROPERTY()
	UWeaponDisplay* WeaponDisplay {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WaveInfoClass;
	UPROPERTY()
	UWaveInfo* WaveInfo{ nullptr };


	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> KillFeedClass;
	UPROPERTY()
	UKillFeed* KillFeed{ nullptr };





//public:
//	AHUDBase();
//
//	virtual void DrawHUD() override;
//
//
//	virtual void ClearHUD() override;
//	virtual void CreateHUD() override;
//	void UpdateHUD();
//
//	UFUNCTION()
//	void UpdateAmmo(int CurrentAmmo, int StoredAmmo);
//
//	UFUNCTION()
//	void UpdateHealth(float CurrentHealth, float MaxHealth);
//
//protected:
//
//
//private:
//	UPROPERTY()
//	UWeaponDisplay* WeaponDisplay {nullptr};
//	UPROPERTY()
//	ACharacterBase* OwningPlayerCharacter {nullptr};
//
//	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
//	TSubclassOf<UUserWidget> WeaponDisplayClass;

};
