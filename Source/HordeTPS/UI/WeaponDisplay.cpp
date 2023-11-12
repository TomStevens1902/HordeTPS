// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDisplay.h"

#include "MathUtil.h"


UWeaponDisplay::UWeaponDisplay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWeaponDisplay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWeaponDisplay::SetCurrentAmmo(FText InText)
{
	CurrentAmmoText->SetText(InText);
}

void UWeaponDisplay::SetStoredAmmo(FText InText)
{
	StoredAmmoText->SetText(InText);
}

void UWeaponDisplay::SetHealth(float CurrentHealth, float MaxHeath)
{
	// Between 0 and 1
	float HealthAsAPercentage = FMathf::Clamp(CurrentHealth / MaxHeath , 0.0f, MaxHeath);
	HealthBar->SetPercent(HealthAsAPercentage);
}



