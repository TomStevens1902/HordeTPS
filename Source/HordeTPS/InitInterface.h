// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORDETPS_API IInitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Init")
	void User_Initialisation();
};
