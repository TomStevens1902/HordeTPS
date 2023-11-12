// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"

#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class HORDETPS_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	explicit AEnemyControllerBase(const FObjectInitializer& ObjectInitializer);
protected:

private:
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnSightStimulus(const FActorPerceptionUpdateInfo& UpdateInfo);
	virtual void OnPossess(APawn* InPawn) override;
	
};
