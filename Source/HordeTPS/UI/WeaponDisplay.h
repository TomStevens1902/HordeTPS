// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HordeTPS/Player/CharacterBase.h"

#include "WeaponDisplay.generated.h"


/**
 * 
 */

class ACharacterBase;

UCLASS()
class HORDETPS_API UWeaponDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UWeaponDisplay(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetCurrentAmmo(FText InText);

	void SetStoredAmmo(FText InText);

	void SetHealth(float CurrentHealth, float MaxHeath);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StoredAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;
};
