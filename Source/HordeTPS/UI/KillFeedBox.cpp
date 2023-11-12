// Fill out your copyright notice in the Description page of Project Settings.


#include "KillFeedBox.h"

void UKillFeedBox::SetKillInfo(FText KillerActor, FText KilledActor)
{
	Killer->SetText(KillerActor);
	Killed->SetText(KilledActor);
}
