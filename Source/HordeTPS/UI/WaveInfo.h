// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WaveInfo.generated.h"

/**
 * 
 */


UCLASS()
class HORDETPS_API UWaveInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWaveNumber(int InNumber);

	void SetCountdownTime(float InTime);
	void OnCountdownEnded();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GameInfo;

	UPROPERTY(Transient, BlueprintReadOnly,meta =(BindWidgetAnim))
	UWidgetAnimation* InfoEnded;

private:
};
