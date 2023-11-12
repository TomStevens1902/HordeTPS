// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDBase.h"

AHUDBase::AHUDBase()
{

}


void AHUDBase::CreateHUD()
{
	if (WeaponDisplayClass)
	{
		WeaponDisplay = CreateWidget<UWeaponDisplay>(GetWorld(), WeaponDisplayClass);
		if (WeaponDisplay)
		{
			WeaponDisplay->AddToViewport();
		}
	}

	if(WaveInfoClass)
	{
		WaveInfo = CreateWidget<UWaveInfo>(GetWorld(), WaveInfoClass);
		if (WaveInfo)
		{
			WaveInfo->AddToViewport();
		}
	}

	if (KillFeedClass)
	{
		KillFeed = CreateWidget<UKillFeed>(GetWorld(), KillFeedClass);
		if (KillFeed)
		{
			KillFeed->AddToViewport();
		}
	}
}

void AHUDBase::Destroyed()
{
	ClearHUD();
	Super::Destroyed();
}


void AHUDBase::DisplayDeath(AActor* KillerActor, AActor* DeadActor)
{
	UKillFeedBox* NewKillFeedBox = KillFeed->AddNewKillFeedBox(KillerActor, DeadActor);
	FTimerHandle TimerHandle_KillFeedBoxLifetime;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(KillFeed, "RemoveKillFeedBox", NewKillFeedBox);

	GetWorldTimerManager().SetTimer(TimerHandle_KillFeedBoxLifetime, Delegate, KillFeed->KillFeedBoxLifetime, false);
}

void AHUDBase::ClearHUD()
{
	if (WeaponDisplay)
	{
	WeaponDisplay->RemoveFromParent();
	}

	if(WaveInfo)
	{
		WaveInfo->RemoveFromParent();
	}

	if (KillFeed)
	{
		KillFeed->RemoveFromParent();
	}
}

void AHUDBase::UpdateAmmo(int CurrentAmmo, int StoredAmmo)
{
	WeaponDisplay->SetCurrentAmmo(FText::FromString(FString::FromInt(CurrentAmmo)));
	WeaponDisplay->SetStoredAmmo(FText::FromString(FString::FromInt(StoredAmmo)));
}

void AHUDBase::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	WeaponDisplay->SetHealth(CurrentHealth, MaxHealth);
}