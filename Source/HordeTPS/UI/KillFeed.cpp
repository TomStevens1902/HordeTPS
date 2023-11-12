// Fill out your copyright notice in the Description page of Project Settings.


#include "KillFeed.h"

#include "KillFeedBox.h"

UKillFeedBox* UKillFeed::AddNewKillFeedBox(AActor* Killer, AActor* Killed)
{
	UKillFeedBox* NewKillFeedBox = CreateWidget<UKillFeedBox>(this, KillFeedBoxClass);
	FText KillerText;
	FText KilledText;

	if(Killer->Implements<UActorInfoInterface>())
	{
		KillerText = Execute_GetActorDisplayName(Killer);
	}
	else
	{
		KillerText = FText::FromString("");
	}

	if (Killed->GetClass()->ImplementsInterface(UActorInfoInterface::StaticClass()))
	{
		KilledText = Execute_GetActorDisplayName(Killed);
	}
	else
	{
		KilledText = FText::FromString("");
	}

	NewKillFeedBox->SetKillInfo(KillerText, KilledText);

	if(KillFeedBoxes.Num() >= 4)
	{
		KillFeedBoxes.RemoveAt(3);
		KillFeedBoxes.Insert(NewKillFeedBox, 0);
		UpdateKillFeed();
	}
	else
	{
		KillFeedBoxes.Insert(NewKillFeedBox, 0);
		UpdateKillFeed();
	}

	return NewKillFeedBox;
}



void UKillFeed::UpdateKillFeed()
{
	KillFeedBox->ClearChildren();
	for(int i{0}; i < KillFeedBoxes.Num(); i++)
	{
		KillFeedBox->AddChild(KillFeedBoxes[i]);
	}
}

void UKillFeed::RemoveKillFeedBox(UKillFeedBox* BoxToRemove)
{
	if(BoxToRemove)
	{
		KillFeedBoxes.Remove(BoxToRemove);
		UpdateKillFeed();
	}
}
