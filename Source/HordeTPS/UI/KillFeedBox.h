// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "KillFeedBox.generated.h"

/**
 * 
 */
UCLASS()
class HORDETPS_API UKillFeedBox : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetKillInfo(FText Killer, FText Killed);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Killer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Cause;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Killed;
private:
};
