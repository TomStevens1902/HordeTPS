// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KillFeedBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "HordeTPS/ActorInfoInterface.h"
#include "KillFeed.generated.h"

/**
 * 
 */

class IActorInfoInterface;


UCLASS()
class HORDETPS_API UKillFeed : public UUserWidget, public IActorInfoInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UKillFeedBox* AddNewKillFeedBox(AActor* Killer, AActor* Killed);
	UFUNCTION()
	void RemoveKillFeedBox(UKillFeedBox* BoxToRemove);
	UPROPERTY()
	float KillFeedBoxLifetime{ 3 };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* KillFeedBox;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> KillFeedBoxClass;

	UPROPERTY()
	TArray<UKillFeedBox*> KillFeedBoxes;

	// The lifetime of the kill feed box in seconds.


	void UpdateKillFeed();

	

};

