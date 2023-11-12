// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveInfo.h"

void UWaveInfo::SetWaveNumber(int InNumber)
{
	FString WaveText = "WAVE ";
	FString Number = FString::FromInt(InNumber);
	WaveText.Append(Number);
	WaveNumber->SetText(FText::FromString(WaveText));
}

void UWaveInfo::SetCountdownTime(float InTime)
{
	if(!GameInfo->IsVisible())
	{
		GameInfo->SetVisibility(ESlateVisibility::Visible);
	}

	FString CountdownText = "NEXT WAVE IN ";
	FString Number = FString::FromInt(ceil(InTime));
	CountdownText.Append(Number);
	GameInfo->SetText(FText::FromString(CountdownText));
}

void UWaveInfo::OnCountdownEnded()
{
	GameInfo->SetText(FText::FromString("WAVE INCOMING"));
	if(InfoEnded)
	{
		PlayAnimation(InfoEnded);
	}
}
