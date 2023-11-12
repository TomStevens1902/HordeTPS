// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORDETPS_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToStoredAmmo(int InAmmo);

	UFUNCTION()
	virtual FVector GetCameraTargetLocation() = 0; //needs to be set to pure for some reason

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReloadNotifyPoint();

};
